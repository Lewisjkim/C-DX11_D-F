#pragma once

#include "CRenderComponent.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       _pAtlas;//***********
    UINT                m_iTileCountX;  // 타일 가로
    UINT                m_iTileCountY;  // 타일 세로

    Vec2                m_vSliceSize;   // 타일 하나의 크기(UV 단위)


    vector<tTile>       m_vecTile;
    CStructuredBuffer* m_Buffer;
    wstring loadstrRelativePath;
public:
    virtual void finaltick() override;
    virtual void render() override;

    void UpdateData();

    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }
    UINT GetTileCountX() const { return m_iTileCountX;}
    UINT GetTileCountY() const { return m_iTileCountY;}
    wstring GetLoadKey() { return loadstrRelativePath; }
public:
    void SaveTile(FILE* _pFile);
    void LoadTile(FILE* _pFile);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};

