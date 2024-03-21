//#pragma once
//#include "CComponent.h"
//
//class CRigidbody :
//    public CComponent
//{
//private:
//    Vec2    m_vForce;           // ��
//    Vec2    m_vVelocity;        // �ӵ� ( vector )
//    float   m_fMass;            // ����
//
//    float   m_fFriction;        // ������
//    float   m_fFrictionScale;   // �������
//
//    float   m_fVelocityLimit;   // ���� �ӵ�
//    float   m_fGravityVLimit;   // �߷¿� ���� ���� �ӵ�
//
//
//    float   m_fGravityAccel;    // �߷� ���ӵ� ����
//    bool    m_bGravityUse;      // �߷� ��뿩��
//    bool    m_bGround;          // �� üũ
//
//
//public:
//    virtual void begin() override;
//    virtual void tick() override;
//    virtual void finaltick() override;
//    virtual void SaveToLevelFile(FILE* _File) override;
//    virtual void LoadFromLevelFile(FILE* _File) override;
//public:
//    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }//��+=��
//    void SetVelocityLimit(float _fLimit) { m_fVelocityLimit = _fLimit; }//�ӵ� ����
//    void SetGravityVelocityLimit(float _fLimit) { m_fGravityVLimit = _fLimit; }// �߷� �ӵ� ����
//
//    void SetVelocity(Vec2 _vVelocity) { m_vVelocity = _vVelocity; }//�ӵ� ����
//    Vec2 GetVelocity() { return m_vVelocity; }
//
//    void AddVelocity(Vec2 _vAddV) { m_vVelocity += _vAddV; }//�ӵ� ��ø
//
//    void SetFriction(float _f) { m_fFriction = _f; }
//    void SetFrictionScale(float _f) { m_fFrictionScale = _f; }
//    void SetGravity(bool _bSet) { m_bGravityUse = _bSet; }
//    void SetGravityAccel(float _fAccel) { m_fGravityAccel = _fAccel; }
//    void SetGround(bool _bGround);
//
//public:
//    CLONE(CRigidbody);
//
//public:
//    CRigidbody();
//    ~CRigidbody();
//};
//
