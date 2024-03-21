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
	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	// Shader Compile	
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", 0, 0, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Compute Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 Shader 를 만든다.
	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize()
		, nullptr, m_CS.GetAddressOf());
}

void CComputeShader::Execute()
{
	UpdateData();	//1.파티클	shader	= 파티클 관련 정보 를 u0 u1 t20에 전달 하고 group 개수 계산	
								//2.setcolorshader = texture정보 u0로 넘기고 group 개수 계산

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);//생성될 때 register 1번으로 세팅됨
	pCB->SetData(&m_Const);//m_Const에 들어간 값을 상수버퍼에 설정
	pCB->UpdateData_CS();//상수 버퍼 내용을 register num에 설정 //noise texture 가로 세로도 상수 쪽에 있다

	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);//장치에 컴퓨트 셰이더를 설정
	CONTEXT->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);//그룹 개수 를 인자로 가지고 실행한다

	Clear();//파티클, setColor 양쪽에서 버퍼및 텍스쳐 들이 사용한 RSV,UAV를 통해 register 내용을 nullptr로 초기화 = 이유 다른 셰이더가 또 사용하기에
}