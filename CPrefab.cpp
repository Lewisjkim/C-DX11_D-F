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

CGameObject* CPrefab::Instantiate()//���ҽ� Ŭ�� xx
{
	return m_ProtoObj->Clone(); 
}

int CPrefab::Save(const wstring& _strRelativePath)
{
	//if (IsEngineRes())//�����Ϸ��� ���׸����� �������� ���� �Ŷ�� return fail
	//	return E_FAIL;

	//wstring strFilePath = CPathMgr::GetInst()->GetContentPath(); //content������ ��θ� �޾ƿ���
	//strFilePath += _strRelativePath; //���� �� �������� ��� ��θ� �߰��Ѵ�
	//CGameObject* prefabobj = m_ProtoObj;

	//FILE* pFile = nullptr;
	//_wfopen_s(&pFile, strFilePath.c_str(), L"wb"); //�� �ش� ����� pFile��  write binary ���� ����

	//// ������ ������Ʈ �̸�
	//SaveWString(prefabobj->GetName(), pFile);

	//// ������ ������Ʈ
	//for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	//{
	//	if (i == (UINT)COMPONENT_TYPE::END)
	//	{
	//		// END ��� ������Ʈ Ÿ�� END ����
	//		fwrite(&i, sizeof(UINT), 1, pFile);
	//		break;
	//	}
	//
	//	//CComponent* Com = prefabobj->GetComponent((COMPONENT_TYPE)i);
	//	//if (nullptr == Com)
	//		//continue;
	//	if(prefabobj->GetComponent((COMPONENT_TYPE)i))
	//		continue;

	//	// ������Ʈ Ÿ�� ����
	//	fwrite(&i, sizeof(UINT), 1, pFile);

	//	// ������Ʈ ���� ����
	//	prefabobj->GetComponent((COMPONENT_TYPE)i)->SaveToLevelFile(pFile);
	//}

	//// ��ũ��Ʈ	
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
	// ���� ������Ʈ�� ���� �Ҽ��� �ƴϿ��� �Ѵ�.
	//assert(18 == _Proto->GetLayerIndex()); 

	m_ProtoObj = _Proto;
}