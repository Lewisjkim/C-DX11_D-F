#include "pch.h"
#include "CDamageScript.h"

CDamageScript::CDamageScript()			:
	CScript((UINT)SCRIPT_TYPE::DAMAGESCRIPT)
{
}

CDamageScript::~CDamageScript()
{
}

void CDamageScript::begin()
{
}

void CDamageScript::tick()
{
	Vec3 pos = Transform()->GetRelativePos();
	pos.y += DT * 80.f;
	Transform()->SetRelativePos(pos);

	Vec3 scale = Transform()->GetRelativeScale();
	if (GetOwner()->GetName() == L"0")
		scale += DT * Vec3(35.f, 35.f, 0.f);
	else if (GetOwner()->GetName() == L"1")
		scale += DT * Vec3(30.f, 30.f, 0.f);
	else if(GetOwner()->GetName() == L"2")
		scale += DT * Vec3(45.f, 45.f, 0.f);
	else if (GetOwner()->GetName() == L"3")
		scale += DT * Vec3(28.f, 28.f, 0.f);
	else 	if(GetOwner()->GetName() == L"4")
		scale += DT * Vec3(41.f, 41.f, 0.f);
	else if (GetOwner()->GetName() == L"5")
		scale += DT * Vec3(33.f, 33.f, 0.f);
	else if(GetOwner()->GetName() == L"6")
		scale += DT * Vec3(25.f, 25.f, 0.f);
	else if (GetOwner()->GetName() == L"7")
		scale += DT * Vec3(50.f, 50.f, 0.f);
	else if(GetOwner()->GetName() == L"8")
		scale += DT * Vec3(48.f, 48.f, 0.f);
	else if (GetOwner()->GetName() == L"9")
		scale += DT * Vec3(30.f, 30.f, 0.f);
	Transform()->SetRelativeScale(scale);
}
