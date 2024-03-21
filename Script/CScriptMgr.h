#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BAKALATTACKSCRIPT,
	BAKALPARTICLE,
	BAKALSCRIPT,
	BAKALSTAMPSCRIPT,
	BREAKOBJECT,
	CAMERAMOVESCRIPT,
	COMBOSCRIPT,
	DAMAGESCRIPT,
	DOORSCRIPT,
	DRUNNERSCRIPT,
	ENTERMAPSCRIPT,
	EXSSCRIPT,
	EZ8SCRIPT,
	FLOATINGLEVEL,
	GOBLINSCRIPT,
	GOBLINTHROWERSCRIPT,
	GTSTONESCRIPT,
	GTSTONESCRIPT2,
	HUDSCRIPT,
	ITEMSCRIPT,
	LOADINGCIRLCE,
	LOGINSCRIPT,
	MISSILESCRIPT,
	PLAYERSCRIPT,
	RX78SCRIPT,
	TAUSCRIPT,
	XSBULLET,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
