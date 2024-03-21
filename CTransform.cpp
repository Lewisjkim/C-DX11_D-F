#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM),
	m_vRelativeScale(Vec3(1.f, 1.f, 1.f)), //������ ���� ������ 0���� ���� �ǹ�����
	m_bAbsolute(false),
	m_vRelativeDir{ Vec3(1.f, 0.f, 0.f), 
					Vec3(0.f, 1.f, 0.f), 
					Vec3(0.f, 0.f, 1.f)} // �ʱⰪ�� finaltick�� �ȵ����� ���� ���ؼ� ����
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	//ũ�����
	m_matWorldScale = XMMatrixIdentity();//return���� ��������� �ʱ�ȭ �Լ�
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);//�������� 0���� ������ִ� �Լ�
	
	//ȸ�� ���
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);// x���� �߽����� ȸ���ϴ� ����� ����ϴ�.
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	//�̵� ���
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);//trans ��� ������ش�

	m_matWorld = m_matWorldScale * matRot * matTranslation;

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot); //���Ϳ� ����� �� ���� ��  �ش� �Լ��� ����Ͽ� ���� ��ǥ(w)�� 0 ����
	}

	// �θ� ������Ʈ Ȯ��
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)// �θ� ������ ����
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			//���� = ���ÿ��� * �θ�ũ�� �� * �θ� ����(ũ��/ȸ��/�̵�)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else //�θ�� ����
		{
			m_matWorldScale = pParent->Transform()->m_matWorldScale;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}
		
		for (int i = 0; i < 3; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}
}

void CTransform::UpdateData()
{
	// ��ġ���� ������ۿ� ���� �� ���ε�		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	//finaltick���� ���� matWorld��ǥ��
	//extern�� ����� g_transform.matworld�� �־��ش�
	//�װ� ������ shader���� ����Ϸ���

	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform); // map unmap���� cpu���� gpu�� �����͸� �ѱ��
	pTransformBuffer->UpdateData(); // pipeline�� ������ �������ش�
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
}
