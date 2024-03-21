#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CPathMgr.h>
#include <Engine\CEventMgr.h>

#include "TreeUI.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"
//ContentUI는 소유하고있는 Obj & Resource를 보여주는 UI

ContentUI::ContentUI()
	: UI("##Content")
{
	SetName("Content");

	// ContentUI 안에 자식으로 Tree 를 추가한다.
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(false);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&ContentUI::SetTargetToInspector);
	m_Tree->SetDragDropID("Resource"); // m_strDragDropID를 Resource로 지정

	AddChildUI(m_Tree);
}

ContentUI::~ContentUI()
{

}

void ContentUI::init()
{
	//ResetContent();
	Reload();
}

void ContentUI::tick()
{
	UI::tick();//필요한가?
	bool a = CResMgr::GetInst()->IsResourceChanged();
	if (CResMgr::GetInst()->IsResourceChanged())
	{
		ResetContent();
	}
}

int ContentUI::render_update()
{
	return 0;
}

void ContentUI::Reload()
{
	// Content 폴더에 있는 파일 이름들을 확인
	m_vecResPath.clear();
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	FindFileName(strContentPath);

	// 파일명으로 리소스 로딩
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		RES_TYPE type = GetResTypeByExt(m_vecResPath[i]);

		if (type == RES_TYPE::END)
			continue;

		switch (type)
		{
		case RES_TYPE::MESHDATA:
			
			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MESH:

			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecResPath[i], m_vecResPath[i]);
			break;

		}
	}

	//리소스의 원본파일 체크
	for (UINT i = 0; i < UINT(RES_TYPE::END); i++)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE) i);
		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes()) //engine resource면 넘어가고
				continue;

			wstring strFilePath = strContentPath + pair.first; //원본 파일이 아닌 파일 중 로딩되지 않은 것
			if (!filesystem::exists(strFilePath)) //파일경로의 파일이 존재한다면
			{
				tEvent evn = {};
				evn.Type = EVENT_TYPE::DELETE_RESOURCE; //지워주는 이벤트를 등록
				evn.wParam = (DWORD_PTR)i;
				evn.lParam = (DWORD_PTR)pair.second.Get();
				CEventMgr::GetInst()->AddEvent(evn);
			}
		}
	}

	//트리 갱신
	ResetContent();
}

void ContentUI::ResetContent()
{
	// Tree Clear
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
	for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		// m_Tree 에 현재 리소스 목록을 AddItem
		TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
		pCategory->SetCategoryNode(true);

		for (const auto& pair : mapRes)
		{
			m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
		}
	}
}

void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CRes* pSelectObject = (CRes*)pSelectedNode->GetData();

	if (nullptr == pSelectObject) //노드가 nullptr이면 안되기에 return
		return;

	// Inspector 에 선택된 Resource 를 알려준다.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetResource(pSelectObject);
}

void ContentUI::FindFileName(const wstring& _FolderPath)
{
	WIN32_FIND_DATA FindData = {};

	wstring FolderPath = _FolderPath + L"*.*";

	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

	while (FindNextFile(hFindHandle, &FindData))
	{
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!wcscmp(FindData.cFileName, L".."))
			{
				continue;
			}

			FindFileName(_FolderPath + FindData.cFileName + L"\\");
			continue;
		}

		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		wstring RelativePath = _FolderPath + FindData.cFileName;
		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

		m_vecResPath.push_back(RelativePath);
	}

	FindClose(hFindHandle);
}

RES_TYPE ContentUI::GetResTypeByExt(const wstring& _relativepath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
	wstring strExt = szExt;

	if (L".mdat" == strExt)
		return RES_TYPE::MESHDATA;
	else if (L".prefab" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpg" == strExt || L".jpeg" == strExt || L".bmp" == strExt || L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".ogg" == strExt)
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}
