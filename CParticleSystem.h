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
    CStructuredBuffer* m_ParticleBuffer;//파티클 개수 사이즈의 버퍼를 생성하고 <== 해당 버퍼 생성 시에 [ 설정한 파티클 정보로 초기화 해준다 ]
    CStructuredBuffer* m_RWBuffer; //파티클 개수 = 데이터를 직접 넘겨주어야 하기 때문에 생성시에 cpu acess를 true로 설정
    CStructuredBuffer*m_ModuleDataBuffer;//변경 되는 파티클들의 정보 = 이 또한 데이터가 변경되어야하기에 생성시에 cpu access를 true로 설정

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

