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

	
	//1.Ÿ���ؽ��ĸ� ���Ͽ��� �ҷ�����
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

		if (false == GetOpenFileName(&ofn))//Ž���� ����
			return true;

		//�ҷ��� �ؽ��ĸ� �̹��� ǥ��

		FILE* pFile = nullptr; // ���� ���Ͽ� ���� pointer�� ������ ���� �����Ϳ� ���� �������Դϴ�.
		_wfopen_s(&pFile, szFilePath, L"rb");//������ ����

		if (nullptr == pFile)
		{
			wchar_t wStr[256] = {};
			MessageBox(nullptr, wStr, L"Ÿ�� �ؽ��� ���� �ҷ����� ����", MB_OK);
		}
		
		CResMgr::GetInst()->LoadAtlas<CTexture>(szFilePath, szFilePath);

		//�ؽ��� Ű�� = CTexture->GetKey()
		//������ ���� = CAnim2D->m_vecFrm.size()

		fclose(pFile);
	}

	//2. �ҷ��� �ؽ��ĵ��� ���(list)����
	const map<wstring, Ptr<CRes>>& mapTile = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);
	ImVec2 LoadTextures = ImVec2(148, 100);
	ImGui::Text("          ");
	ImGui::SameLine();
	if (ImGui::BeginListBox("##Loaded Textures", LoadTextures))
	{
		for (int i = 0; i < m_vecStrData.size(); i++)
		{
			const bool is_selected = (m_iSelectedIdx == i);

			// ����Ʈ ������ ���, ��ȯ���� Ŭ�� True/ False
			if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
			{
				m_iSelectedIdx = i;
			}

			// ListUI�� Ư�� ������(�ε��� i) ������ ����Ŭ���� �߻��ϸ�
			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))//����Ʈ �����ۿ� �ö� �ѹ� Ŭ����
			{
				
				// Select Delegate �� ��ϵǾ��ִٸ� ȣ���� �ش�.
				if (m_SelectInst && m_SelectDelegate)
				{
					//stringData�� ���ڷ� ���� Delegate ȣ��
					(m_SelectInst->*m_SelectDelegate)((DWORD_PTR)m_vecStrData[i].c_str());
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
			
		}
		//��� ����
		m_vecStrData.clear();
		for (const auto& pair : mapTile)
		{
			this->AddTileItem(string(pair.first.begin(), pair.first.end()));
			
		}
		ImGui::EndListBox();

	}
	AddTileDynamic_Select(this, (UI_DELEGATE_1)&TileMapUI::SelectTileTexture);//��������Ʈ ���

	//3.Ÿ���� ���� & ���� �� �Է�
	ImGui::Text("Row       ");
	ImGui::SameLine();
	Row = 1;
	ImGui::InputInt("##Row", &Row);

	ImGui::Text("Col       ");
	ImGui::SameLine();
	Col = 1;
	ImGui::InputInt("##Col", &Col);
	
	//4.Ÿ�� ������ ����
	ImGui::Text("SliceSizeX");
	ImGui::SameLine();
	SliceSizeX = 1;
	ImGui::InputFloat("##SliceSizeX", &SliceSizeX);

	ImGui::Text("SliceSizeY");
	ImGui::SameLine();
	SliceSizeY = 1;
	ImGui::InputFloat("##SliceSizeY", &SliceSizeY);

	//���Ͽ��� �ҷ����ؽ��ĸ� Material->Texparam�� ���
	if (ImGui::Button("Resize"))
	{
		GetTarget()->TileMap()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(szFilePath));
		GetTarget()->TileMap()->SetSliceSize(Vec2(SliceSizeX, SliceSizeY));
		GetTarget()->TileMap()->SetTileCount(Row, Col);

	}
	

	
		
	//6. .tileȮ���ڷ� �����ϱ�
	
	ImGui::SameLine();
	if(ImGui::Button("Save"))
		TileSave();
	
	//7. .tileȮ���ڷ� ������ Ÿ�� �ҷ�����
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		TileLoad();
	}
	return FALSE;
	return TRUE;
}

void TileMapUI::SelectTileTexture(DWORD_PTR _Key)//Textyre  �� Key��
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
	ofn.lpstrFile = szFilePath;//�����̸�
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

	// Ÿ�� ����
	fwrite(&iTileXCount, sizeof(int), 1, pFile);
	fwrite(&iTileYCount, sizeof(int), 1, pFile);
	// Ÿ�� ũ��
	fwrite(&SliceX, sizeof(float), 1, pFile);
	fwrite(&SliceY, sizeof(float), 1, pFile);

	//Ÿ�� ��ġ, ũ��, �ؽ���, ���, �ؽ��� Ű��
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

	// Ÿ�� ���� ���� ���� �б�
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
	
	// �ٽ� Ÿ�� ��ġ
	Vec2 vSliceSize = Vec2(SliceX, SliceY);
	GetTarget()->Transform()->SetRelativePos(vPos);

	GetTarget()->Transform()->SetRelativeScale(vScale);
	GetTarget()->TileMap()->SetSliceSize(vSliceSize);
	GetTarget()->TileMap()->SetTileCount(iTileXCount, iTileYCount);
	
	// ��Ʋ�� �̹��� ����
	GetTarget()->TileMap()->LoadTile(pFile);
	
	
	fclose(pFile);
}
