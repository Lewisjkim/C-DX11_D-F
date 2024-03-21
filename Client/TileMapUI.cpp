#include "pch.h"
#include "TileMapUI.h"
#include <Engine\CGameObject.h>
#include <Engine/CTileMap.h>
#include <Engine/CTransform.h>
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CMaterial.h>
#include "commdlg.h"
#include "InspectorUI.h"

TileMapUI::TileMapUI()		:
	ComponentUI("##TileMap", COMPONENT_TYPE::TILEMAP)//m_strID
{
	SetName("TileMap");//m_strName
}

TileMapUI::~TileMapUI()
{
}

int TileMapUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	
	//1.타일텍스쳐를 파일에서 불러오고
	ImGui::Text("          ");
	ImGui::SameLine();
	if (ImGui::Button("Load Tile Atlas", ImVec2(148, 18)))
	{
		OPENFILENAME ofn = {};

		wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
		strFolderPath += L"texture\\";

		

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"All Files\0*.*\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strFolderPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))//탐색기 종료
			return true;

		//불러온 텍스쳐를 이미지 표시

		FILE* pFile = nullptr; // 열린 파일에 대한 pointer를 수신할 파일 포인터에 대한 포인터입니다.
		_wfopen_s(&pFile, szFilePath, L"rb");//파일을 연다

		if (nullptr == pFile)
		{
			wchar_t wStr[256] = {};
			MessageBox(nullptr, wStr, L"타일 텍스쳐 파일 불러오기 실패", MB_OK);
		}
		
		CResMgr::GetInst()->LoadAtlas<CTexture>(szFilePath, szFilePath);

		//텍스쳐 키값 = CTexture->GetKey()
		//프레임 정보 = CAnim2D->m_vecFrm.size()

		fclose(pFile);
	}

	//2. 불러온 텍스쳐들의 목록(list)나열
	const map<wstring, Ptr<CRes>>& mapTile = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);
	ImVec2 LoadTextures = ImVec2(148, 100);
	ImGui::Text("          ");
	ImGui::SameLine();
	if (ImGui::BeginListBox("##Loaded Textures", LoadTextures))
	{
		for (int i = 0; i < m_vecStrData.size(); i++)
		{
			const bool is_selected = (m_iSelectedIdx == i);

			// 리스트 아이템 출력, 반환값은 클릭 True/ False
			if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
			{
				m_iSelectedIdx = i;
			}

			// ListUI내 특정 아이템(인덱스 i) 위에서 더블클릭이 발생하면
			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))//리스트 아이템에 올라가 한번 클릭시
			{
				
				// Select Delegate 가 등록되어있다면 호출해 준다.
				if (m_SelectInst && m_SelectDelegate)
				{
					//stringData를 인자로 받은 Delegate 호출
					(m_SelectInst->*m_SelectDelegate)((DWORD_PTR)m_vecStrData[i].c_str());
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
			
		}
		//목록 나열
		m_vecStrData.clear();
		for (const auto& pair : mapTile)
		{
			this->AddTileItem(string(pair.first.begin(), pair.first.end()));
			
		}
		ImGui::EndListBox();

	}
	AddTileDynamic_Select(this, (UI_DELEGATE_1)&TileMapUI::SelectTileTexture);//델리게이트 등록

	//3.타일의 가로 & 세로 값 입력
	ImGui::Text("Row       ");
	ImGui::SameLine();
	Row = 1;
	ImGui::InputInt("##Row", &Row);

	ImGui::Text("Col       ");
	ImGui::SameLine();
	Col = 1;
	ImGui::InputInt("##Col", &Col);
	
	//4.타일 사이즈 설정
	ImGui::Text("SliceSizeX");
	ImGui::SameLine();
	SliceSizeX = 1;
	ImGui::InputFloat("##SliceSizeX", &SliceSizeX);

	ImGui::Text("SliceSizeY");
	ImGui::SameLine();
	SliceSizeY = 1;
	ImGui::InputFloat("##SliceSizeY", &SliceSizeY);

	//파일에서 불러온텍스쳐를 Material->Texparam에 등록
	if (ImGui::Button("Resize"))
	{
		GetTarget()->TileMap()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(szFilePath));
		GetTarget()->TileMap()->SetSliceSize(Vec2(SliceSizeX, SliceSizeY));
		GetTarget()->TileMap()->SetTileCount(Row, Col);

	}
	

	
		
	//6. .tile확장자로 저장하기
	
	ImGui::SameLine();
	if(ImGui::Button("Save"))
		TileSave();
	
	//7. .tile확장자로 저장한 타일 불러오기
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		TileLoad();
	}
	return FALSE;
	return TRUE;
}

void TileMapUI::SelectTileTexture(DWORD_PTR _Key)//Textyre  의 Key값
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTile = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->TileMap()->GetMaterial()->SetTexParam(TEX_0, pTile);
}

void TileMapUI::TileSave()
{
	// open a file name
	OPENFILENAME ofn = {};

	wstring strTileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strTileFolderPath += L"texture\\";


	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;//파일이름
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strTileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"wb");

	
	int iTileXCount = GetRow();
	int iTileYCount = GetCol();
	float SliceX = GetSliceSizeX();
	float SliceY = GetSliceSizeY();

	// 타일 개수
	fwrite(&iTileXCount, sizeof(int), 1, pFile);
	fwrite(&iTileYCount, sizeof(int), 1, pFile);
	// 타일 크기
	fwrite(&SliceX, sizeof(float), 1, pFile);
	fwrite(&SliceY, sizeof(float), 1, pFile);

	//타일 위치, 크기, 텍스쳐, 경로, 텍스쳐 키값
	GetTarget()->TileMap()->SaveTile(pFile);
		
	//Load<CTexture>(L"TileAtlasTex", L"texture\\TILE.bmp");
	
	fclose(pFile);
}

void TileMapUI::TileLoad()
{
	// open a file name
	OPENFILENAME ofn = {};

	wstring strTileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strTileFolderPath += L"texture\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strTileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;


	FILE* pFile = nullptr;
	_wfopen_s(&pFile, szFilePath, L"rb");

	// 타일 개로 세로 개수 읽기
	int iTileXCount = 0, iTileYCount = 0;
	Vec3 vPos = {};
	Vec3 vScale = {};
	float SliceX = 0.f;
	float SliceY = 0.f;
	
	fread(&iTileXCount, sizeof(int), 1, pFile);
	fread(&iTileYCount, sizeof(int), 1, pFile);
	fread(&SliceX, sizeof(float), 1, pFile);
	fread(&SliceY, sizeof(float), 1, pFile);
	fread(&vPos, sizeof(Vec3), 1, pFile);
	fread(&vScale, sizeof(Vec3), 1, pFile);
	
	// 다시 타일 배치
	Vec2 vSliceSize = Vec2(SliceX, SliceY);
	GetTarget()->Transform()->SetRelativePos(vPos);

	GetTarget()->Transform()->SetRelativeScale(vScale);
	GetTarget()->TileMap()->SetSliceSize(vSliceSize);
	GetTarget()->TileMap()->SetTileCount(iTileXCount, iTileYCount);
	
	// 아틀라스 이미지 정보
	GetTarget()->TileMap()->LoadTile(pFile);
	
	
	fclose(pFile);
}
