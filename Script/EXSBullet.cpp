#include "pch.h"
#include "EXSBullet.h"
#include "CEXSScript.h"

EXSBullet::EXSBullet() :
	CScript((UINT)SCRIPT_TYPE::XSBULLET),
	Speed(300.f),
	DurationTime(2.f)
{
}

EXSBullet::~EXSBullet()
{
}

void EXSBullet::begin()
{
	SetLifeSpan(DurationTime);
	Ptr<CSound> s_EXSbullet = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\bullet_explosive_none_02.ogg");
	s_EXSbullet->Stop();
	s_EXSbullet->Play(1, 0.2f, false);
}

void EXSBullet::tick()
{
	
	Vec3 bulletpos = Transform()->GetRelativePos();
	if (b_dir == false)
	{
		Vec3 rot = Vec3(0.f, -180.f, 0.f);
		rot = (rot / 180.f) * XM_PI;
		GetOwner()->Transform()->SetRelativeRot(rot);
		bulletpos.x -= DT * Speed;
	}
	else
	{
		bulletpos.x += DT * Speed;
	}

	
	Transform()->SetRelativePos(bulletpos);
	
}

void EXSBullet::BeginOverlap(CCollider2D* _Other)
{
}

void EXSBullet::OnOverlap(CCollider2D* _Other)
{
}

void EXSBullet::EndOverlap(CCollider2D* _Other)
{
}


