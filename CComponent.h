#pragma once
#include "CEntity.h"
#include "CGameObject.h"

class CTransform;
class CMeshRender;

#define GET_OTHER_COMPONENT(Type) C##Type* Type() {return m_pOwner->Type();}

class CComponent :
    public CEntity
{
private:
    CGameObject*            m_pOwner;// 소유하고 있는 GameObject
    const COMPONENT_TYPE    m_Type;// Component Type

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_pOwner; }

public:
    virtual void begin() {}
    virtual void tick() {} 
    virtual void finaltick() = 0;
    virtual CComponent* Clone() = 0;//순수가상함수 = 정의가 이뤄지지 않고 함수만 선언된 것 이게 있는 순가 추상 클래스
           
public:
    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _File) = 0;

public:
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Animator2D);

public:
    CComponent(COMPONENT_TYPE _Type);
    CComponent(const CComponent& _Other);
    virtual ~CComponent();

    friend class CGameObject; //obj에서 불러다 쓰려고
};

