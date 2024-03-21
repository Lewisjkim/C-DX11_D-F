#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CPathMgr.h"
#include "CResMgr.h"

CMaterial::CMaterial(bool _bEngine)
	: CRes(RES_TYPE::MATERIAL, _bEngine)
	, m_Const{}
	, m_arrTex{}
{
}

CMaterial::~CMaterial()
{
}


void CMaterial::UpdateData()
{
	if (nullptr == m_pShader)
		return;

	m_pShader->UpdateData();//���̴��� �� �ܰ迡 ������Ʈ


	// �ؽ��� Update
	for (UINT i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_arrTex[i])//�ؽ��İ� ���ٸ�
		{
			m_Const.arrTex[i] = 0;//�ؽ��� ������ 0 ����
			CTexture::Clear(i);//static Clear�� ����register num�� ���� ����ϰ� ������
			continue;
		}

		else //�ؽ��İ� �ִٸ�
		{
			m_Const.arrTex[i] = 1;//�ؽ��� ������ 1 ���� shader ������ true false�� ��� �Ϸ���
			m_arrTex[i]->UpdateData(i, PIPELINE_STAGE::PS_PIXEL);//levelmgr���� ���� �ؽ��� �ּҸ� ps�ܰ迡 �ѱ��
		}
	}

	// Constant Update
	CConstBuffer* pMtrlBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pMtrlBuffer->SetData(&m_Const);
	pMtrlBuffer->UpdateData();
}


void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const void* _Src)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		m_Const.arrInt[_Param] = *((int*)_Src);// imgui���� �������� �Է� ���� ���� struct�� ����
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_Const.arrFloat[_Param - FLOAT_0] = *((float*)_Src);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_Const.arrV2[_Param - VEC2_0] = *((Vec2*)_Src);
		break;

	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_Const.arrV4[_Param - VEC4_0] = *((Vec4*)_Src);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_Const.arrMat[_Param - MAT_0] = *((Matrix*)_Src);
		break;
	}
}

void CMaterial::SetTexParam(TEX_PARAM _Param, const Ptr<CTexture>& _Tex)
{
	m_arrTex[_Param] = _Tex;
}

void CMaterial::GetScalarParam(SCALAR_PARAM _param, void* _pData)//�ش� �Ķ���Ϳ� ������ ���� ���ڷ� ������
{
	switch (_param)//�� �Ķ���Ϳ� �°�
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
	{
		int idx = (UINT)_param - (UINT)INT_0;//�ش� �ε����� ����ؼ�
		*((int*)_pData) = m_Const.arrInt[idx];//�Ѱ��� �ش� ����ü ���� �������� ������ ���� �־��ش�
	}
	break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
	{
		int idx = (UINT)_param - (UINT)FLOAT_0;
		*((float*)_pData) = m_Const.arrFloat[idx];
	}
	break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
	{
		int idx = (UINT)_param - (UINT)VEC2_0;
		*((Vec2*)_pData) = m_Const.arrV2[idx];
	}
	break;

	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
	{
		int idx = (UINT)_param - (UINT)VEC4_0;
		*((Vec4*)_pData) = m_Const.arrV4[idx];
	}
	break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
	{
		int idx = (UINT)_param - (UINT)MAT_0;
		*((Matrix*)_pData) = m_Const.arrMat[idx];
	}
	break;
	}
}


// ================
// File Save / Load
// ================
int CMaterial::Save(const wstring& _strRelativePath) // ���׸����� ��� ��θ� ���ڷ� ������
{
	if (IsEngineRes())//�����Ϸ��� ���׸����� �������� ���� �Ŷ�� return fail
		return E_FAIL;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath(); //content������ ��θ� �޾ƿ���
	strFilePath += _strRelativePath; //������ ���׸����� ��� ��θ� �߰��Ѵ�

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb"); //�� �ش� ����� pFile��  write binary ���� ����

	// Entity
	SaveWString(GetName(), pFile); //���׸����� �̸� ����

	// Res
	SaveWString(GetKey(), pFile); // ���ҽ��� Ű���� ����

	// Shader
	SaveResRef(m_pShader.Get(), pFile);

	//Constant
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

	//Texture
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; i++)
	{
		SaveResRef(m_arrTex[i].Get(), pFile);
	}


	fclose(pFile);
	return S_OK;
}


int CMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// Entity
	wstring strName;
	LoadWString(strName, pFile); //���Ͽ��� ���� ���׸��� �̸��� strName���޾ƿ���
	SetName(strName); // �̸����� ����

	// Res
	wstring strKey;
	LoadWString(strKey, pFile); // ���Ͽ��� ���� Ű���� strKey�� �޾ƿ���

	// Shader
	LoadResRef(m_pShader, pFile);

	//constant
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

	//Texture
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; i++)
	{
		LoadResRef(m_arrTex[i], pFile);
	}

	

	fclose(pFile);

	return 0;

	return S_OK;
}