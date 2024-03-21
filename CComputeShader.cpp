#include "pch.h"
#include "CComputeShader.h"

#include "CPathMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CComputeShader::CComputeShader()		:
	CShader(RES_TYPE::COMPUTE_SHADER),
	m_iGroupX(1),
	m_iGroupY(1),
	m_iGroupZ(1)
{
}

CComputeShader::~CComputeShader()
{
}

void CComputeShader::CreateComputeShader(const wstring& _strFileName, const string& _strFuncName)
{
	// Shader ���� ���
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	// Shader Compile	
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", 0, 0, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Compute Shader Compile Failed!!", MB_OK);
	}

	// �����ϵ� ��ü�� Shader �� �����.
	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize()
		, nullptr, m_CS.GetAddressOf());
}

void CComputeShader::Execute()
{
	UpdateData();	//1.��ƼŬ	shader	= ��ƼŬ ���� ���� �� u0 u1 t20�� ���� �ϰ� group ���� ���	
								//2.setcolorshader = texture���� u0�� �ѱ�� group ���� ���

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);//������ �� register 1������ ���õ�
	pCB->SetData(&m_Const);//m_Const�� �� ���� ������ۿ� ����
	pCB->UpdateData_CS();//��� ���� ������ register num�� ���� //noise texture ���� ���ε� ��� �ʿ� �ִ�

	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);//��ġ�� ��ǻƮ ���̴��� ����
	CONTEXT->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);//�׷� ���� �� ���ڷ� ������ �����Ѵ�

	Clear();//��ƼŬ, setColor ���ʿ��� ���۹� �ؽ��� ���� ����� RSV,UAV�� ���� register ������ nullptr�� �ʱ�ȭ = ���� �ٸ� ���̴��� �� ����ϱ⿡
}