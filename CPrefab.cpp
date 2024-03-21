#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"


CPrefab::CPrefab()
	: CRes(RES_TYPE::PREFAB)
	, m_ProtoObj(nullptr)
{
}

CPrefab::~CPrefab()
{
	//if (nullptr != m_ProtoObj)

		//delete m_ProtoObj;
}

CGameObject* CPrefab::Instantiate()//리소스 클론 xx
{
	return m_ProtoObj->Clone(); 
}

int CPrefab::Save(const wstring& _strRelativePath)
{
	//if (IsEngineRes())//저장하려는 마테리얼이 엔진에서 만든 거라면 return fail
	//	return E_FAIL;

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath(); //content까지의 경로를 받아오고
	//strFilePath += _strRelativePath; //저장 할 프리펩의 상대 경로를 추가한다
	//CGameObject* prefabobj = m_ProtoObj;

	//FILE* pFile = nullptr;
	//_wfopen_s(&pFile, strFilePath.c_str(), L"wb"); //위 해당 경로의 pFile를  write binary 모드로 연다

	//// 프리펩 오브젝트 이름
	//SaveWString(prefabobj->GetName(), pFile);

	//// 보유한 컴포넌트
	//for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	//{
	//	if (i == (UINT)COMPONENT_TYPE::END)
	//	{
	//		// END 라면 컴포넌트 타입 END 저장
	//		fwrite(&i, sizeof(UINT), 1, pFile);
	//		break;
	//	}
	//
	//	//CComponent* Com = prefabobj->GetComponent((COMPONENT_TYPE)i);
	//	//if (nullptr == Com)
	//		//continue;
	//	if(prefabobj->GetComponent((COMPONENT_TYPE)i))
	//		continue;

	//	// 컴포넌트 타입 저장
	//	fwrite(&i, sizeof(UINT), 1, pFile);

	//	// 컴포넌트 정보 저장
	//	prefabobj->GetComponent((COMPONENT_TYPE)i)->SaveToLevelFile(pFile);
	//}

	//// 스크립트	
	//const vector<CScript*>& vecScript = m_ProtoObj->GetScripts();
	//size_t ScriptCount = vecScript.size();
	//fwrite(&ScriptCount, sizeof(size_t), 1, pFile);

	//for (size_t i = 0; i < vecScript.size(); ++i)
	//{
	//	wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
	//	SaveWString(ScriptName, pFile);
	//	vecScript[i]->SaveToLevelFile(pFile);
	//}
	return 0;
}

void CPrefab::RegisterProtoObject(CGameObject* _Proto)
{
	// 원본 오브젝트는 레벨 소속이 아니여야 한다.
	//assert(18 == _Proto->GetLayerIndex()); 

	m_ProtoObj = _Proto;
}