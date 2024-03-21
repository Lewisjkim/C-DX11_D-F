#include "pch.h"
#include "CSetColorShader.h"

#include "CTexture.h"

CSetColorShader::CSetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CSetColorShader::~CSetColorShader()
{
}

void CSetColorShader::UpdateData()
{
	m_OutTex->UpdateData_CS(0, false);//텍스쳐 정보를 register u0에 설정하고 computeshader에 설정

	// 그룹 개수 계산
	m_iGroupX = m_OutTex->Width() / m_iGroupPerThreadX;
	m_iGroupY = m_OutTex->Height() / m_iGroupPerThreadY;
	m_iGroupZ = 1;
}

void CSetColorShader::Clear()
{
	m_OutTex->Clear();
}
