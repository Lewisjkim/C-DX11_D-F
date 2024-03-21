#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"
#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;//��ƼŬ ���� �������� ���۸� �����ϰ� <== �ش� ���� ���� �ÿ� [ ������ ��ƼŬ ������ �ʱ�ȭ ���ش� ]
    CStructuredBuffer* m_RWBuffer; //��ƼŬ ���� = �����͸� ���� �Ѱ��־�� �ϱ� ������ �����ÿ� cpu acess�� true�� ����
    CStructuredBuffer*m_ModuleDataBuffer;//���� �Ǵ� ��ƼŬ���� ���� = �� ���� �����Ͱ� ����Ǿ���ϱ⿡ �����ÿ� cpu access�� true�� ����

    tParticleModule                             m_ModuleData;
    Ptr<CParticleUpdateShader>      m_UpdateCS;
    Ptr<CTexture>                                pParticleTex;

    float                       m_AccTime;

    

public:
    
    virtual void finaltick() override;
    virtual void render() override;
    Ptr<CTexture> GetParticleTex() { return pParticleTex; }

    CStructuredBuffer* GetModuleDataBuffer() { return m_ModuleDataBuffer; }
    void SetModuleData(tParticleModule _ModuleData) { m_ModuleData = _ModuleData; }
    tParticleModule GetModuleData() { return m_ModuleData; }
    Ptr<CParticleUpdateShader> GetUpdateCS() { return m_UpdateCS; }

public:
    virtual void SaveToLevelFile(FILE* _File)override;
    virtual void LoadFromLevelFile(FILE* _File)override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    ~CParticleSystem();
};

