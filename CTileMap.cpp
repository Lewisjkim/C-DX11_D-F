#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CPathMgr.h"

#include "CStructuredBuffer.h"
#include "CLevelMgr.h"
#include "CLevel.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iTileCountX(1)
	, m_iTileCountY(1)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_Buffer = new CStructuredBuffer;
	m_Buffer->Create(sizeof(tTile), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_ONLY, true);
}

CTileMap::~CTileMap()
{
	if (nullptr != m_Buffer)
		delete m_Buffer;
	
}

void CTileMap::finaltick()
{
}

void CTileMap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// ���� ������Ʈ
	GetMaterial()->SetScalarParam(INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(INT_1, &m_iTileCountY);
	GetMaterial()->UpdateData();

	// ����ȭ���� ������Ʈ
	UpdateData();

	// ����
	GetMesh()->render();
}

void CTileMap::UpdateData()
{
	m_Buffer->SetData(m_vecTile.data(), sizeof(tTile) * m_vecTile.size());
	m_Buffer->UpdateData(20, PIPELINE_STAGE::PS_PIXEL);
}

void CTileMap::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	m_iTileCountX = _iXCount;
	m_iTileCountY = _iYCount;

	m_vecTile.clear();
	m_vecTile.resize(m_iTileCountX * m_iTileCountY);

	if (m_Buffer->GetElementCount() < m_vecTile.size())
	{
		m_Buffer->Create(sizeof(tTile), (UINT)m_vecTile.size(), SB_TYPE::READ_ONLY, true);
	}

	// Ÿ�� ���� �׽�Ʈ
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{
			m_vecTile[i * m_iTileCountX + j].vLeftTop.x = m_vSliceSize.x * j;//���� ��ġx
			m_vecTile[i * m_iTileCountX + j].vLeftTop.y = m_vSliceSize.y * i;//���� ��ġy
			m_vecTile[i * m_iTileCountX + j].vSlice = m_vSliceSize;
		}
	}
}

void CTileMap::SaveTile(FILE* _pFile)
{
	// ��ġ & ũ�� & �ؽ��� & Ÿ��ũ�� 
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	fwrite(&vPos, sizeof(Vec3), 1, _pFile);//��ġ
	fwrite(&vScale, sizeof(Vec3), 1, _pFile);//ũ��

	_pAtlas = GetOwner()->TileMap()->GetMaterial()->GetTexParam(TEX_0);
	bool bAtlas = false;
	if(nullptr != _pAtlas)
		bAtlas = true;
	fwrite(&bAtlas, sizeof(bool), 1, _pFile);//�ؽ��� ����

	if (bAtlas)
	{
		// Ű�� ����,
		wstring strKey = _pAtlas->GetKey();
		size_t iLen = strKey.length();
		fwrite(&iLen, sizeof(size_t), 1, _pFile);
		fwrite(strKey.c_str(), sizeof(wchar_t), iLen, _pFile);

		// ����� ����,
		wstring strRelativePath = _pAtlas->GetRelativePath();
		size_t iLenPath = strRelativePath.length();
		fwrite(&iLenPath, sizeof(size_t), 1, _pFile);
		fwrite(strRelativePath.c_str(), sizeof(wchar_t), iLenPath, _pFile);

		fwrite(&m_vSliceSize, sizeof(Vec2), 1, _pFile);
		fwrite(&m_iTileCountX, sizeof(UINT), 1, _pFile);
		fwrite(&m_iTileCountY, sizeof(UINT), 1, _pFile);
	}

	
}

void CTileMap::LoadTile(FILE* _pFile)
{
	
	//��Ʋ��, Ű��, ���
	bool bAtlas =false;
	fread(&bAtlas, sizeof(bool), 1, _pFile);

	if (bAtlas)
	{
		wstring strKey;

		//key load
		size_t iLen = 0;
		fread(&iLen, sizeof(size_t), 1, _pFile);
		
		wchar_t szBuff[256] = {};
		fread(szBuff, sizeof(wchar_t), iLen, _pFile); //Ű��(����̸��� ����)
		strKey = szBuff;

		//Path
		size_t iLenPath = 0;
		fread(&iLenPath, sizeof(size_t), 1, _pFile);

		wchar_t szBuffPath[256] = {};
		fread(szBuffPath, sizeof(wchar_t), iLenPath, _pFile);
		fread(&m_vSliceSize, sizeof(Vec2), 1, _pFile);
		fread(&m_iTileCountX, sizeof(UINT), 1, _pFile);
		fread(&m_iTileCountY, sizeof(UINT), 1, _pFile);
		loadstrRelativePath = szBuffPath;
		CResMgr::GetInst()->LoadAtlas<CTexture>(strKey, loadstrRelativePath);
		Ptr<CTexture> pTile = CResMgr::GetInst()->FindRes<CTexture>(strKey);

		
		GetMaterial()->SetTexParam(TEX_0, pTile);
		SetSliceSize(m_vSliceSize);
		SetTileCount(m_iTileCountX, m_iTileCountY);
		//GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(szFilePath));
	}
}

void CTileMap::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	//���� ���õ� �ؽ���
	_pAtlas = GetOwner()->TileMap()->GetMaterial()->GetTexParam(TEX_0);
	bool bAtlas = false;
	if (nullptr != _pAtlas)
		bAtlas = true;
	fwrite(&bAtlas, sizeof(bool), 1, _File);//�ؽ��� ����

	//���� �ִϸ��̼��� ������ ����

	if (bAtlas)
	{
		// Ű�� ����,
		wstring strKey = _pAtlas->GetKey();
		size_t iLen = strKey.length();
		fwrite(&iLen, sizeof(size_t), 1, _File);
		fwrite(strKey.c_str(), sizeof(wchar_t), iLen, _File);

		// ����� ����,
		wstring strRelativePath = _pAtlas->GetRelativePath();
		size_t iLenPath = strRelativePath.length();
		fwrite(&iLenPath, sizeof(size_t), 1, _File);
		fwrite(strRelativePath.c_str(), sizeof(wchar_t), iLenPath, _File);
		fwrite(&m_vSliceSize, sizeof(Vec2), 1, _File);
		fwrite(&m_iTileCountX, sizeof(UINT), 1, _File);
		fwrite(&m_iTileCountY, sizeof(UINT), 1, _File);
	}
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	LoadTile(_File);

}