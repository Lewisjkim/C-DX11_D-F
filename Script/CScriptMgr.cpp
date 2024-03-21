#include "pch.h"
#include "CScriptMgr.h"

#include "CBakalAttackScript.h"
#include "CBakalParticle.h"
#include "CBakalScript.h"
#include "CBakalStampScript.h"
#include "CBreakObject.h"
#include "CCameraMoveScript.h"
#include "CComboScript.h"
#include "CDamageScript.h"
#include "CDoorScript.h"
#include "CDRunnerScript.h"
#include "CEnterMapScript.h"
#include "CEXSScript.h"
#include "CEZ8Script.h"
#include "CFloatingLevel.h"
#include "CGoblinScript.h"
#include "CGoblinThrowerScript.h"
#include "CGTStoneScript.h"
#include "CGTStoneScript2.h"
#include "CHudScript.h"
#include "CItemScript.h"
#include "CLoadingCirlce.h"
#include "CLoginScript.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"
#include "CRX78Script.h"
#include "CTauScript.h"
#include "EXSBullet.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBakalAttackScript");
	_vec.push_back(L"CBakalParticle");
	_vec.push_back(L"CBakalScript");
	_vec.push_back(L"CBakalStampScript");
	_vec.push_back(L"CBreakObject");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CComboScript");
	_vec.push_back(L"CDamageScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CDRunnerScript");
	_vec.push_back(L"CEnterMapScript");
	_vec.push_back(L"CEXSScript");
	_vec.push_back(L"CEZ8Script");
	_vec.push_back(L"CFloatingLevel");
	_vec.push_back(L"CGoblinScript");
	_vec.push_back(L"CGoblinThrowerScript");
	_vec.push_back(L"CGTStoneScript");
	_vec.push_back(L"CGTStoneScript2");
	_vec.push_back(L"CHudScript");
	_vec.push_back(L"CItemScript");
	_vec.push_back(L"CLoadingCirlce");
	_vec.push_back(L"CLoginScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRX78Script");
	_vec.push_back(L"CTauScript");
	_vec.push_back(L"EXSBullet");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBakalAttackScript" == _strScriptName)
		return new CBakalAttackScript;
	if (L"CBakalParticle" == _strScriptName)
		return new CBakalParticle;
	if (L"CBakalScript" == _strScriptName)
		return new CBakalScript;
	if (L"CBakalStampScript" == _strScriptName)
		return new CBakalStampScript;
	if (L"CBreakObject" == _strScriptName)
		return new CBreakObject;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CComboScript" == _strScriptName)
		return new CComboScript;
	if (L"CDamageScript" == _strScriptName)
		return new CDamageScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CDRunnerScript" == _strScriptName)
		return new CDRunnerScript;
	if (L"CEnterMapScript" == _strScriptName)
		return new CEnterMapScript;
	if (L"CEXSScript" == _strScriptName)
		return new CEXSScript;
	if (L"CEZ8Script" == _strScriptName)
		return new CEZ8Script;
	if (L"CFloatingLevel" == _strScriptName)
		return new CFloatingLevel;
	if (L"CGoblinScript" == _strScriptName)
		return new CGoblinScript;
	if (L"CGoblinThrowerScript" == _strScriptName)
		return new CGoblinThrowerScript;
	if (L"CGTStoneScript" == _strScriptName)
		return new CGTStoneScript;
	if (L"CGTStoneScript2" == _strScriptName)
		return new CGTStoneScript2;
	if (L"CHudScript" == _strScriptName)
		return new CHudScript;
	if (L"CItemScript" == _strScriptName)
		return new CItemScript;
	if (L"CLoadingCirlce" == _strScriptName)
		return new CLoadingCirlce;
	if (L"CLoginScript" == _strScriptName)
		return new CLoginScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRX78Script" == _strScriptName)
		return new CRX78Script;
	if (L"CTauScript" == _strScriptName)
		return new CTauScript;
	if (L"EXSBullet" == _strScriptName)
		return new EXSBullet;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BAKALATTACKSCRIPT:
		return new CBakalAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::BAKALPARTICLE:
		return new CBakalParticle;
		break;
	case (UINT)SCRIPT_TYPE::BAKALSCRIPT:
		return new CBakalScript;
		break;
	case (UINT)SCRIPT_TYPE::BAKALSTAMPSCRIPT:
		return new CBakalStampScript;
		break;
	case (UINT)SCRIPT_TYPE::BREAKOBJECT:
		return new CBreakObject;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::COMBOSCRIPT:
		return new CComboScript;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGESCRIPT:
		return new CDamageScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::DRUNNERSCRIPT:
		return new CDRunnerScript;
		break;
	case (UINT)SCRIPT_TYPE::ENTERMAPSCRIPT:
		return new CEnterMapScript;
		break;
	case (UINT)SCRIPT_TYPE::EXSSCRIPT:
		return new CEXSScript;
		break;
	case (UINT)SCRIPT_TYPE::EZ8SCRIPT:
		return new CEZ8Script;
		break;
	case (UINT)SCRIPT_TYPE::FLOATINGLEVEL:
		return new CFloatingLevel;
		break;
	case (UINT)SCRIPT_TYPE::GOBLINSCRIPT:
		return new CGoblinScript;
		break;
	case (UINT)SCRIPT_TYPE::GOBLINTHROWERSCRIPT:
		return new CGoblinThrowerScript;
		break;
	case (UINT)SCRIPT_TYPE::GTSTONESCRIPT:
		return new CGTStoneScript;
		break;
	case (UINT)SCRIPT_TYPE::GTSTONESCRIPT2:
		return new CGTStoneScript2;
		break;
	case (UINT)SCRIPT_TYPE::HUDSCRIPT:
		return new CHudScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSCRIPT:
		return new CItemScript;
		break;
	case (UINT)SCRIPT_TYPE::LOADINGCIRLCE:
		return new CLoadingCirlce;
		break;
	case (UINT)SCRIPT_TYPE::LOGINSCRIPT:
		return new CLoginScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::RX78SCRIPT:
		return new CRX78Script;
		break;
	case (UINT)SCRIPT_TYPE::TAUSCRIPT:
		return new CTauScript;
		break;
	case (UINT)SCRIPT_TYPE::XSBULLET:
		return new EXSBullet;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BAKALATTACKSCRIPT:
		return L"CBakalAttackScript";
		break;

	case SCRIPT_TYPE::BAKALPARTICLE:
		return L"CBakalParticle";
		break;

	case SCRIPT_TYPE::BAKALSCRIPT:
		return L"CBakalScript";
		break;

	case SCRIPT_TYPE::BAKALSTAMPSCRIPT:
		return L"CBakalStampScript";
		break;

	case SCRIPT_TYPE::BREAKOBJECT:
		return L"CBreakObject";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::COMBOSCRIPT:
		return L"CComboScript";
		break;

	case SCRIPT_TYPE::DAMAGESCRIPT:
		return L"CDamageScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::DRUNNERSCRIPT:
		return L"CDRunnerScript";
		break;

	case SCRIPT_TYPE::ENTERMAPSCRIPT:
		return L"CEnterMapScript";
		break;

	case SCRIPT_TYPE::EXSSCRIPT:
		return L"CEXSScript";
		break;

	case SCRIPT_TYPE::EZ8SCRIPT:
		return L"CEZ8Script";
		break;

	case SCRIPT_TYPE::FLOATINGLEVEL:
		return L"CFloatingLevel";
		break;

	case SCRIPT_TYPE::GOBLINSCRIPT:
		return L"CGoblinScript";
		break;

	case SCRIPT_TYPE::GOBLINTHROWERSCRIPT:
		return L"CGoblinThrowerScript";
		break;

	case SCRIPT_TYPE::GTSTONESCRIPT:
		return L"CGTStoneScript";
		break;

	case SCRIPT_TYPE::GTSTONESCRIPT2:
		return L"CGTStoneScript2";
		break;

	case SCRIPT_TYPE::HUDSCRIPT:
		return L"CHudScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"CItemScript";
		break;

	case SCRIPT_TYPE::LOADINGCIRLCE:
		return L"CLoadingCirlce";
		break;

	case SCRIPT_TYPE::LOGINSCRIPT:
		return L"CLoginScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::RX78SCRIPT:
		return L"CRX78Script";
		break;

	case SCRIPT_TYPE::TAUSCRIPT:
		return L"CTauScript";
		break;

	case SCRIPT_TYPE::XSBULLET:
		return L"EXSBullet";
		break;

	}
	return nullptr;
}