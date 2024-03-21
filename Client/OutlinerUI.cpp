#include "pch.h"
#include "OutlinerUI.h"
#include "commdlg.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CEventMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine/components.h>
#include <Engine/CResMgr.h>
#include <Script/CScriptMgr.h>
#include <Engine/CScript.h>

#include "CLevelSaveLoad.h"

#include "TreeUI.h"

OutlinerUI::OutlinerUI()
	: UI("##Outliner")
	, m_Tree(nullptr)
{
	SetName("Outliner");

	// OutlinerUI 안에 자식으로 Tree 를 추가한다.
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(false);
	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&OutlinerUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&OutlinerUI::DragDrop);//tree로 Delegate_2 DragDrop함수를 사용한다고 등록
	m_Tree->SetDragDropID("GameObject"); // m_strDragDropID를 GameObject로 설정
	m_Tree->AddDynamic_ChangeName(this, (UI_DELEGATE_1)&OutlinerUI::ChangeObjectName);//선택한 노드 이름 바꾸는 델리게이트
	m_Tree->AddDynamic_CreatePrefab(this, (UI_DELEGATE_1)&OutlinerUI::CreatePrefab);

	AddChildUI(m_Tree);

}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::tick()
{
	
	if (CEventMgr::GetInst()->IsLevelChanged())
	{
		ResetOutliner();

		// 트리 리셋 후, 다음 선택노드로 지정된 DATA 가 있으면, 찾아서 선택노드로 변경
		if (0 != m_dwSelectedData)
		{
			m_Tree->GetSelectedNode(m_dwSelectedData);
		}
	}
}

int OutlinerUI::render_update()
{
	return 0;
}

void OutlinerUI::ResetOutliner() //모든 오브젝트를 트리에 넣고 노드를 생성시키고 주소를 받아
{
	// Tree Clear
	m_Tree->Clear(); //기존에 열려있던 트리를 지워주고
	m_Tree->AddItem("Root", 0);

	// 레벨 매니저에서 현재 모든 레벨 목록 받아옴
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);

		const vector<CGameObject*>& vecParentObj = pLayer->GetParentObject();

		for (size_t i = 0; i < vecParentObj.size(); ++i)
		{
			AddGameObject(vecParentObj[i], nullptr);
			/*m_Tree->AddItem(string(vecParentObj[i]->GetName().begin()
				, vecParentObj[i]->GetName().end())
				, (DWORD_PTR)vecParentObj[i]);*/
		}
	}
}

void OutlinerUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CGameObject* pSelectObject = (CGameObject*)pSelectedNode->GetData();

	// Inspector 에 선택된 GameObject 를 알려준다.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pSelectObject);
}

void OutlinerUI::AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode)
{
	// 오브젝트를 트리에 넣고, 생성된 노드 주소를 받아둔다.
	TreeNode* pNode = m_Tree->AddItem(string(_Obj->GetName().begin(), _Obj->GetName().end())
		, (DWORD_PTR)_Obj
		, _ParentNode);

	// 오브젝트의 자식오브젝트 들을 오브젝트 노드를 부모로 해서 그 밑으로 다시 넣어준다.
	const vector<CGameObject*>& vecChild = _Obj->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(vecChild[i], pNode);
	}
}

CGameObject* OutlinerUI::GetSelectedObject()
{
	TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

	if (nullptr == pSelectedNode)
		return nullptr;

	return (CGameObject*)pSelectedNode->GetData();
}


void OutlinerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_DropNode;


	CGameObject* pDragObj = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObj = nullptr;
	if (nullptr != pDropNode)
	{
		pDropObj = (CGameObject*)pDropNode->GetData();
	}

	// 자식으로 들어갈 오브젝트가 목적지 오브젝트의 조상(부모계층) 중 하나라면, 
	// AddChild 처리하지 않는다.
	if (nullptr != pDropObj)
	{
		if (pDropObj->IsAncestor(pDragObj))
			return;
	}

	// 이벤트 매니저를 통해서 처리한다.
	tEvent evn = {};
	evn.Type = EVENT_TYPE::ADD_CHILD;
	evn.wParam = (DWORD_PTR)pDropObj;
	evn.lParam = (DWORD_PTR)pDragObj;
	CEventMgr::GetInst()->AddEvent(evn);
}
//마우스 우클릭 하면 -> Text입력 란으로 바뀌고->ChangObjectName()로 해당 노드의 이름을 Setting해준다
void OutlinerUI::ChangeObjectName(DWORD_PTR _SelectNode)
{
	TreeNode* SelectNode = (TreeNode*)_SelectNode;
	string name = SelectNode->GetName(); // 현재 GameObject나옴
	if (ImGui::Begin("##namepopup"))
	{
		ImGui::InputText("##ChangeName", (char*)name.c_str(), name.length(), 0);
		ImGui::End();
	}
	CGameObject* pSelectObj = (CGameObject*)SelectNode->GetData(); // 선택된 노드의 주소값을 받아
	string ChangedName = name;
	wstring wChangedName = wstring(ChangedName.begin(), ChangedName.end());
	pSelectObj->SetName(wChangedName);
}

void OutlinerUI::CreatePrefab(DWORD_PTR _SelectNode)
{
	TreeNode* SelectNode = (TreeNode*)_SelectNode;
	//wstring prefabname = wstring(SelectNode->GetName().begin(), SelectNode->GetName().end());
	
	CGameObject* pSelectObj = (CGameObject*)SelectNode->GetData(); //원본 오브젝트 주소
		
	//프리펩 을 프리펩 생성
	/*Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->RegisterProtoObject(pSelectObj);
	CResMgr::GetInst()->AddRes<CPrefab>(prefabname, pPrefab);*/
	OPENFILENAME ofn = {};

	wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	strFolderpath += L"texture\\Prefab\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Prefab\0*.prefab\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strFolderpath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

	int ind = pSelectObj->GetLayerIndex();
	fwrite(&ind, sizeof(int), 1, pFile);
	CLevelSaveLoad::SaveGameObject(pSelectObj, pFile);
	
	fclose(pFile);
	
}