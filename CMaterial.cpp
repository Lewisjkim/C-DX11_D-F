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

	m_pShader->UpdateData();//쉐이더를 각 단계에 업데이트


	// 텍스쳐 Update
	for (UINT i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_arrTex[i])//텍스쳐가 없다면
		{
			m_Const.arrTex[i] = 0;//텍스쳐 변수에 0 대입
			CTexture::Clear(i);//static Clear로 기존register num에 들어간거 깔끔하게 지워줌
			continue;
		}

		else //텍스쳐가 있다면
		{
			m_Const.arrTex[i] = 1;//텍스쳐 변수에 1 대입 shader 측에서 true false로 사용 하려고
			m_arrTex[i]->UpdateData(i, PIPELINE_STAGE::PS_PIXEL);//levelmgr에서 넣은 텍스쳐 주소를 ps단계에 넘긴다
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
		m_Const.arrInt[_Param] = *((int*)_Src);// imgui에서 수동으로 입력 받은 값을 struct에 적용
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

void CMaterial::GetScalarParam(SCALAR_PARAM _param, void* _pData)//해당 파라미터와 데이터 값이 인자로 들어오면
{
	switch (_param)//그 파라미터에 맞게
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
	{
		int idx = (UINT)_param - (UINT)INT_0;//해당 인덱스를 계산해서
		*((int*)_pData) = m_Const.arrInt[idx];//넘겨질 해당 구조체 값을 역참조한 데이터 값에 넣어준다
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
int CMaterial::Save(const wstring& _strRelativePath) // 마테리얼의 상대 경로를 인자로 받으면
{
	if (IsEngineRes())//저장하려는 마테리얼이 엔진에서 만든 거라면 return fail
		return E_FAIL;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath(); //content까지의 경로를 받아오고
	strFilePath += _strRelativePath; //저장할 마테리얼의 상대 경로를 추가한다

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb"); //위 해당 경로의 pFile를  write binary 모드로 연다

	// Entity
	SaveWString(GetName(), pFile); //마테리얼의 이름 저장

	// Res
	SaveWString(GetKey(), pFile); // 리소스의 키값을 저장

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
	LoadWString(strName, pFile); //파일에서 부터 마테리얼 이름을 strName에받아오고
	SetName(strName); // 이름으로 설정

	// Res
	wstring strKey;
	LoadWString(strKey, pFile); // 파일에서 부터 키값을 strKey에 받아오고

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