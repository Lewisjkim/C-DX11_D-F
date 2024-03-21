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

	// OutlinerUI �ȿ� �ڽ����� Tree �� �߰��Ѵ�.
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(false);
	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&OutlinerUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&OutlinerUI::DragDrop);//tree�� Delegate_2 DragDrop�Լ��� ����Ѵٰ� ���
	m_Tree->SetDragDropID("GameObject"); // m_strDragDropID�� GameObject�� ����
	m_Tree->AddDynamic_ChangeName(this, (UI_DELEGATE_1)&OutlinerUI::ChangeObjectName);//������ ��� �̸� �ٲٴ� ��������Ʈ
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

		// Ʈ�� ���� ��, ���� ���ó��� ������ DATA �� ������, ã�Ƽ� ���ó��� ����
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

void OutlinerUI::ResetOutliner() //��� ������Ʈ�� Ʈ���� �ְ� ��带 ������Ű�� �ּҸ� �޾�
{
	// Tree Clear
	m_Tree->Clear(); //������ �����ִ� Ʈ���� �����ְ�
	m_Tree->AddItem("Root", 0);

	// ���� �Ŵ������� ���� ��� ���� ��� �޾ƿ�
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

	// Inspector �� ���õ� GameObject �� �˷��ش�.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pSelectObject);
}

void OutlinerUI::AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode)
{
	// ������Ʈ�� Ʈ���� �ְ�, ������ ��� �ּҸ� �޾Ƶд�.
	TreeNode* pNode = m_Tree->AddItem(string(_Obj->GetName().begin(), _Obj->GetName().end())
		, (DWORD_PTR)_Obj
		, _ParentNode);

	// ������Ʈ�� �ڽĿ�����Ʈ ���� ������Ʈ ��带 �θ�� �ؼ� �� ������ �ٽ� �־��ش�.
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

	// �ڽ����� �� ������Ʈ�� ������ ������Ʈ�� ����(�θ����) �� �ϳ����, 
	// AddChild ó������ �ʴ´�.
	if (nullptr != pDropObj)
	{
		if (pDropObj->IsAncestor(pDragObj))
			return;
	}

	// �̺�Ʈ �Ŵ����� ���ؼ� ó���Ѵ�.
	tEvent evn = {};
	evn.Type = EVENT_TYPE::ADD_CHILD;
	evn.wParam = (DWORD_PTR)pDropObj;
	evn.lParam = (DWORD_PTR)pDragObj;
	CEventMgr::GetInst()->AddEvent(evn);
}
//���콺 ��Ŭ�� �ϸ� -> Text�Է� ������ �ٲ��->ChangObjectName()�� �ش� ����� �̸��� Setting���ش�
void OutlinerUI::ChangeObjectName(DWORD_PTR _SelectNode)
{
	TreeNode* SelectNode = (TreeNode*)_SelectNode;
	string name = SelectNode->GetName(); // ���� GameObject����
	if (ImGui::Begin("##namepopup"))
	{
		ImGui::InputText("##ChangeName", (char*)name.c_str(), name.length(), 0);
		ImGui::End();
	}
	CGameObject* pSelectObj = (CGameObject*)SelectNode->GetData(); // ���õ� ����� �ּҰ��� �޾�
	string ChangedName = name;
	wstring wChangedName = wstring(ChangedName.begin(), ChangedName.end());
	pSelectObj->SetName(wChangedName);
}

void OutlinerUI::CreatePrefab(DWORD_PTR _SelectNode)
{
	TreeNode* SelectNode = (TreeNode*)_SelectNode;
	//wstring prefabname = wstring(SelectNode->GetName().begin(), SelectNode->GetName().end());
	
	CGameObject* pSelectObj = (CGameObject*)SelectNode->GetData(); //���� ������Ʈ �ּ�
		
	//������ �� ������ ����
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

	// ���� �����
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

	int ind = pSelectObj->GetLayerIndex();
	fwrite(&ind, sizeof(int), 1, pFile);
	CLevelSaveLoad::SaveGameObject(pSelectObj, pFile);
	
	fclose(pFile);
	
}