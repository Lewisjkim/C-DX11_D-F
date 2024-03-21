#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CRes.h"



CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleData{}
	, m_AccTime(0.f)
{
	m_ModuleData.iMaxParticleCount = 1000;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = false;
	m_ModuleData.SpawnRate = 2;
	m_ModuleData.vSpawnColor = Vec3(1.f, 0.4f, 0.4f);
	m_ModuleData.vSpawnScaleMin = Vec3(5.f, 5.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(5.f, 5.f, 1.f);
	m_ModuleData.SpawnShapeType = 0;
	//m_ModuleData.fSphereShapeRadius = 2.f;
	m_ModuleData.vBoxShapeScale = Vec3(500.f, 500.f, 200.f);
	m_ModuleData.Space = 0; // �ùķ��̼� ��ǥ��
	m_ModuleData.MinLifeTime = 10.5f;
	m_ModuleData.MaxLifeTime = 10.5f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
	m_ModuleData.StartScale = 1.5f;
	m_ModuleData.EndScale = 0.2f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
	m_ModuleData.vStartColor = Vec4(1.f, 0.4f, 0.4f,0.2f);
	m_ModuleData.vEndColor = Vec4(0.4f, 0.3f, 1.f,0.2f);

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
	m_ModuleData.AddVelocityType = 3; // From Center, To Center, Fixed Direction
	m_ModuleData.Speed = 300.f;
	m_ModuleData.vVelocityDir = Vec4(1,1,0,0);
	m_ModuleData.OffsetAngle ;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_ModuleData.StartDrag = 500.f;
	m_ModuleData.EndDrag = -200.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_ModuleData.fNoiseTerm = 0.3f;
	m_ModuleData.fNoiseForce = 50.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = false;
	m_ModuleData.VelocityAlignment = true; // �̵��ϴ� �������� x�� ����
	m_ModuleData.VelocityScale = true; // �ӵ��� ���� ���� ��ȭ
	m_ModuleData.vMaxVelocityScale = Vec3(10.f, 2.f, 1.f);
	m_ModuleData.vMaxSpeed = 700.f;


	// ���� �޽�
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// ��ƼŬ ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);

	
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_RWBuffer)
		delete m_RWBuffer;

	if (nullptr != m_ModuleDataBuffer)
		delete m_ModuleDataBuffer;
}


void CParticleSystem::finaltick()
{
	// ���� ����Ʈ ���
	// 1�� ���� �ð�
	float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	m_AccTime += DT;

	// �����ð��� ���� �����ð��� �Ѿ��
	if (fTimePerCount < m_AccTime)
	{
		// �ʰ� ���� ==> ���� ����
		float fData = m_AccTime / fTimePerCount;

		// �������� ���� �ð�
		m_AccTime = fTimePerCount * (fData - floor(fData));

		// ���ۿ� ���� ī��Ʈ ����
		tRWParticleBuffer rwbuffer = { (int)fData, };
		m_RWBuffer->SetData(&rwbuffer);
	}


	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�
	m_ModuleDataBuffer->SetData(&m_ModuleData);

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
	m_UpdateCS->SetModuleData(m_ModuleDataBuffer);
	m_UpdateCS->SetNoiseTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Noise\\noise_01.png"));
	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());

	m_UpdateCS->Execute();

	
		//pParticleTex = CResMgr::GetInst()->Load<CTexture>(getKey, getRPath);
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	// ��ƼŬ���� t20 �� ���ε�
	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	// ��� ������ t21 �� ���ε�
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);

	// Particle Render	
	pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_0", L"texture\\particle\\AlphaCircle.png");
	GetMaterial()->SetTexParam(TEX_0, pParticleTex);
	
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);

	// ��ƼŬ ���� ���ε� ����
	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	/*fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY], sizeof(int), 1, _File);
	fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE], sizeof(int), 1, _File);
	fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG], sizeof(int), 1, _File);
	fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE], sizeof(int), 1, _File);
	fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN], sizeof(int), 1, _File);
	fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER], sizeof(int), 1, _File);
	fwrite(&m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE], sizeof(int), 1, _File);

	fwrite(&m_ModuleData.AddVelocityType, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.StartDrag, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.EndDrag, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.StartScale, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.EndScale, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.fNoiseForce, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.fNoiseTerm, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.fSphereShapeRadius, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.iMaxParticleCount, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.MinLifeTime, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.MaxLifeTime, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.OffsetAngle, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.Space, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.SpawnRate, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.SpawnShapeType, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.Speed, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.vBoxShapeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_ModuleData.VelocityAlignment, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.VelocityScale, sizeof(int), 1, _File);
	fwrite(&m_ModuleData.vStartColor, sizeof(Vec4), 1, _File);
	fwrite(&m_ModuleData.vEndColor, sizeof(Vec4), 1, _File);
	fwrite(&m_ModuleData.vMaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_ModuleData.vMaxVelocityScale, sizeof(Vec4), 1, _File);
	fwrite(&m_ModuleData.vSpawnColor, sizeof(Vec4), 1, _File);
	fwrite(&m_ModuleData.vSpawnScaleMin, sizeof(Vec4), 1, _File);
	fwrite(&m_ModuleData.vSpawnScaleMax, sizeof(Vec4), 1, _File);
	fwrite(&m_ModuleData.vVelocityDir, sizeof(Vec4), 1, _File);*/

	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	SaveResRef(m_UpdateCS.Get(), _File);
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);

	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();
	}
}