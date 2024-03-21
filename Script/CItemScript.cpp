#include "pch.h"
#include "CItemScript.h"

CItemScript::CItemScript() :
	CScript((UINT)SCRIPT_TYPE::ITEMSCRIPT),
	coinPrice(100),
	itemCount(1)
{
}

CItemScript::~CItemScript()
{
}

void CItemScript::begin()
{
}

void CItemScript::tick()
{
}

void CItemScript::BeginOverlap(CCollider2D* _Other)
{
}

void CItemScript::OnOverlap(CCollider2D* _Other)
{
}

void CItemScript::EndOverlap(CCollider2D* _Other)
{
}
