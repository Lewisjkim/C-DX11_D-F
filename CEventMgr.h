#pragma once
#include "CSingleton.h"
class CEventMgr :
    public CSingleton<CEventMgr>
{
    SINGLE(CEventMgr)
private:
    vector<tEvent>          m_vecEvent;
    vector<CGameObject*>    m_vecGC;//가베지 컬렉터 vector<CGameObject>

    bool                    m_LevelChanged;

public:
    void AddEvent(const tEvent& _evn) { m_vecEvent.push_back(_evn); }
    
    bool IsLevelChanged() { return m_LevelChanged; }
private:
    void GC_Clear();

public: 
    void tick();

};

