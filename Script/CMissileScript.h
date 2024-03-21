#pragma once
#include <Engine/CScript.h>
#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
class CPlayerScript;
class CMissileScript :
    public CScript
{
private:
    /*CPlayerScript* p_Script;*/
    Vec3    m_vDir;     //이동 방향
    float    m_fSpeed;   //이동 속력
    float    m_time;
    Vec3    vPos;
    Vec3    playerpos;
    bool    b_dir;
    int        Damage;
public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }
    void SetBulletDamage(int PAttack) { Damage = PAttack; }
    int GetBulletDamage() { return Damage; }

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

public:
    CLONE(CMissileScript)
        Vec3 GetPlayerPos()
    {
        CLayer* playerlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2);
        vector<CGameObject*> vecplayer = playerlayer->GetParentObject();
        for (size_t i = 0; i < vecplayer.size(); i++)
        {
            playerpos = vecplayer[i]->Transform()->GetRelativePos();
        }
        return playerpos;
    }
public:
    CMissileScript();
    ~CMissileScript();
};

