#pragma once
#include "ComponentUI.h"

class TileMapUI :
    public ComponentUI
{
private:
    /*Ptr<CTexture>   m_TileAtlas;*/
    vector<string>  m_vecStrData;//����Ʈ �̸�
    int             m_iSelectedIdx;     //���õ� ������
    UI* m_SelectInst;       // selectable�� ȣ�� ������ ui ��ü��
    UI_DELEGATE_1   m_SelectDelegate;   //�̰� ȣ�� ��Ű�� �ȴ�
    int Row = 0;
    int Col = 0;
    float SliceSizeX = 0;
    float SliceSizeY = 0;
    wchar_t szFilePath[256] = {};
    wchar_t file_name[256] = {};
public:
    virtual int render_update() override;
    void AddTileItem(const string& _strItem) { m_vecStrData.push_back(_strItem);    }
    void AddTileDynamic_Select(UI* _Inst, UI_DELEGATE_1 _Func) { m_SelectInst = _Inst; m_SelectDelegate = _Func; }
private:
    void SelectTileTexture(DWORD_PTR _Key); //��������Ʈ�� �ҷ��� �Լ�
    int GetRow() { return Row; }
    int GetCol() { return Col; }
    float GetSliceSizeX() { return SliceSizeX; }
    float GetSliceSizeY() { return SliceSizeY; }
    void TileSave();
    void TileLoad();
public:
    TileMapUI();
    ~TileMapUI();
};

