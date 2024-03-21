#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;
//레벨들이 공통적으로 가져야 할 것들
class CLevel :
    public CEntity
{
private:
    CLayer*             m_arrLayer[MAX_LAYER];
    LEVEL_STATE         m_State;

public:
    void begin();
    void tick(); // 32개 레이어에게 tick을 준다
    void finaltick();

public:
    CLayer* FindLayerByName(const wstring& _strName);
    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }

    CGameObject* FindObjectByName(const wstring& _Name);
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _Out);

    void AddGameObject(CGameObject* _Object, int _iLayerIdx, bool _bMove);
    void AddGameObject(CGameObject* _Object, const wstring& _LayerName, bool _Move);

    void ChangeState(LEVEL_STATE _State);
    LEVEL_STATE GetState() { return m_State; }

private:
    // 등록된 GameObject 제거
    void clear();

    CLONE(CLevel);

public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

