#pragma once
#include "ComponentUI.h"
#include <Engine/ptr.h>
#include <Engine/CRenderComponent.h>
#include <Engine/CParticleUpdateShader.h>
class CStructuredBuffer;

class ParticleSystemUI :
    public ComponentUI
{
private:
    tParticleModule m_Module;
    bool SpawnModuleOn;
    bool ColorChangeModuleOn;
    bool ScaleChangeModuleOn;
    bool AddVelocityModuleOn;
    bool DragModuleOn;
    bool NoiseForceModuleOn;
    bool RenderModuleOn;
    int MPC;

    //float fSpawnColor[3] = { 0, 0, 0};
    int SpawnRate = 0;
    //float fSpawnScaleMin[3] = { 0,0,0 };
    //float fSpawnScaleMax[3] = { 0, 0, 0 };
    int SpawnShapeType = 0;
   // float fBoxShapeScale[3] = { 0, 0, 0 };
    float SphereShapeRadius = 0.f;
    int Space = 0;
    float MinLifeTime = 0.f;
    float MaxLifeTime = 0.f;

    //float StartColor[4] = { 0,0,0,0 };
   // float EndColor[4] = { 0,0,0,0 };

    float StartScale = 0.f;
    float EndScale = 0.f;

   // float fVelocityDir[4];
    int AddVelocityType = 0;
    float OffsetAngle = 0.f;
    float Speed = 0.f;

    float StartDrag = 0.f;
    float EndDrag = 0.f;

    float NoiseTerm = 0.f;
    float NoiseForce = 0.f;

    int VelocityAlignment = 0;
    int VelocityScale = 0;
    float MaxSpeed = 0.f;
    //float MaxVelocityScale[4] = { 0.f,0.f,0.f,0.f };

    Ptr<CParticleUpdateShader>      m_UpdateCS;
    Ptr<CTexture> ParticleTex;
    float                       m_AccTime;
private:
    void SaveParticle();
    void LoadParticle();
    void ParticleInputs();
    virtual int render_update() override;
public:
    tParticleModule GetModulefromUI() { return m_Module; }
public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

