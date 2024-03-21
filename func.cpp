#include "pch.h"

#include "CEventMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "func.h"
#include "ptr.h"
#include "CResMgr.h"
#include "/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"
void SpawnPrefab(wstring _relativepath, Vec3 _vWorldPos,float time)
{
	wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	wstring relativepath = _relativepath;
	strFolderpath += relativepath;
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");
	int ind = 0;
	fread(&ind, sizeof(int), 1, pFile);
	CGameObject* newPrefab = CLevelSaveLoad::LoadGameObject(pFile);
	Vec3 prefpos = _vWorldPos;

	SpawnGameObject(newPrefab, _vWorldPos, ind);
	if(time != 0)
		newPrefab->SetLifeSpan(time);
	fclose(pFile);
}
CGameObject* SpawnPrefab(wstring _relativepath, Vec3 _vWorldPos)
{
	wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	wstring relativepath = _relativepath;
	strFolderpath += relativepath;
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");
	int ind = 0;
	fread(&ind, sizeof(int), 1, pFile);
	CGameObject* newPrefab = CLevelSaveLoad::LoadGameObject(pFile);
	Vec3 prefpos = _vWorldPos;

	SpawnGameObject(newPrefab, _vWorldPos, ind);
	fclose(pFile);
	return newPrefab;
}

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;//추가되는 오브젝트 주소 값
	evn.lParam = _LayerIdx;//원하는 레이어 인덱스

	CEventMgr::GetInst()->AddEvent(evn);
}

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_LayerName)->GetLayerIndex();//해당 이름에 대한 레이어 인덱스

	CEventMgr::GetInst()->AddEvent(evn);
}

void DestroyObject(CGameObject* _DeletObject)
{
	if (_DeletObject->IsDead())
		return;

	tEvent evn = {};

	evn.Type = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)_DeletObject;

	CEventMgr::GetInst()->AddEvent(evn);
}

int GetDigitCount(int Damage)
{
	int count = 0;
	while (Damage != 0)//자리수 구하기
	{
		Damage = Damage / 10;
		count++;
	}
	return count;
}

void ShowDamage(int Damage, int DigitCount, CGameObject* owner)
{
	Vec3 textpos = owner->Transform()->GetRelativePos();
	textpos += Vec3(0.f, 30.f, 0.f);
	while (Damage != 0 )
	{
		DigitCount--;
		int digit = pow(10, DigitCount);
		int num = Damage / digit;
		textpos += Vec3(30.f, 0.f, 0.f);
		textpos.z = 0.f;
		if (num == 1)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red1.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"1");
		}
		else if (num == 2)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red2.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"2");
		}
		else if (num == 3)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red3.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"3");
		}
		else if (num == 4)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red4.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"4");
		}
		else if (num == 5)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red5.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"5");
		}
		else if (num == 6)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red6.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"6");
		}
		else if (num == 7)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red7.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"7");
		}
		else if (num == 8)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red8.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"8");
		}
		else if (num == 9)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red9.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"9");
		}
		else if (num == 0)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red0.prefab", textpos);
			DamageText->SetLifeSpan(1.f);
			DamageText->SetName(L"0");
		}
		
		Damage = Damage % digit;// 100%3 알
		if (digit>0 && 0 == Damage)
		{
			int num = GetDigitCount(digit);
			num -= 1;
			for (size_t i = 0; i < num; i++)
			{
				textpos += Vec3(30.f, 0.f, 0.f);
				CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\Red0.prefab", textpos);
				DamageText->SetLifeSpan(1.f);
				DamageText->SetName(L"0");
			}
		}
	}
}

void ShowCombo(int combo, int DigitCount, CGameObject* comboObject)
{
	Vec3 textpos = comboObject->Transform()->GetRelativePos();
	textpos.x = textpos.x -400.f;
	while (combo != 0)
	{
		DigitCount--;
		int digit = pow(10, DigitCount);
		int num = combo / digit;// 100/3
		textpos += Vec3(30.f, 0.f, 0.f);
		textpos.z = 0.f;
		if (num == 1)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo1.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c1");
		}
		else if (num == 2)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo2.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c2");
		}
		else if (num == 3)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo3.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c3");
		}
		else if (num == 4)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo4.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c4");
		}
		else if (num == 5)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo5.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c5");
		}
		else if (num == 6)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo6.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c6");
		}
		else if (num == 7)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo7.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c7");
		}
		else if (num == 8)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo8.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c8");
		}
		else if (num == 9)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo9.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c9");
		}
		else if (num == 0)
		{
			CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo0.prefab", textpos);
			DamageText->SetLifeSpan(0.5f);
			DamageText->SetName(L"c0");
		}

		combo = combo % digit;// 100%3 알
		if (digit > 0 && 0 == combo)
		{
			int num = GetDigitCount(digit);
			num -= 1;
			for (size_t i = 0; i < num; i++)
			{
				textpos += Vec3(30.f, 0.f, 0.f);
				CGameObject* DamageText = SpawnPrefab(L"texture\\Prefab\\combo0.prefab", textpos);
				DamageText->SetLifeSpan(0.5f);
				DamageText->SetName(L"c0");
			}
		}
	}
}

void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

//void SaveWString(const wstring& _str, FILE* _pFile)
//{
//	size_t iLen = _str.length();
//	fwrite(&iLen, sizeof(size_t), 1, _pFile);
//	fwrite(_str.c_str(), sizeof(wchar_t), iLen, _pFile);
//}
//
//void LoadWString(wstring& _str, FILE* _pFile)
//{
//	size_t iLen = 0;
//	fread(&iLen, sizeof(size_t), 1, _pFile);
//
//	wchar_t szBuff[256] = {};
//	fread(szBuff, sizeof(wchar_t), iLen, _pFile);
//	_str = szBuff;
//}



const char* ToString(RES_TYPE type)
{
	return RES_TYPE_STR[(UINT)type];
}

const wchar_t* ToWString(RES_TYPE type)
{
	return RES_TYPE_WSTR[(UINT)type];
}

const char* ToString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_STR[(UINT)type];
}
const wchar_t* ToWSTring(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_WSTR[(UINT)type];
}

const char* ToString(RS_TYPE type)
{
	return RS_TYPE_STR[(UINT)type];
}
const wchar_t* ToWSTring(RS_TYPE type)
{
	return RS_TYPE_WSTR[(UINT)type];
}

const char* ToString(DS_TYPE type)
{
	return DS_TYPE_STR[(UINT)type];
}
const wchar_t* ToWSTring(DS_TYPE type)
{
	return DS_TYPE_WSTR[(UINT)type];
}

const char* ToString(BS_TYPE type)
{
	return BS_TYPE_STR[(UINT)type];
}
const wchar_t* ToWSTring(BS_TYPE type)
{
	return BS_TYPE_WSTR[(UINT)type];
}

const char* ToString(SHADER_DOMAIN type)
{
	return DOMAIN_TYPE_STR[(UINT)type];
}
const wchar_t* ToWSTring(SHADER_DOMAIN type)
{
	return DOMAIN_TYPE_WSTR[(UINT)type];
}

void SaveWString(const wstring& _str, FILE* _File)
{
	UINT iLen = (UINT)_str.length(); // 인자로 받아온 이름.길이를 받아서
	fwrite(&iLen, sizeof(UINT), 1, _File); // 길이를 UINT 크기 / 1개 만큼 _File에 쓴다
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File); // 이름을 wchar_t크기 / 이름길이 만큼 _File에 쓴다
}

void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[256] = {};

	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _File); // iLen에 UINT크기 / 1개 만큼 _File에서 읽어온다 ( 길이)
	fread(szBuffer, sizeof(wchar_t), iLen, _File); // szBuffer에 wchar_t크기 / iLen개수만큼 _File에서 읽어온다

	_str = szBuffer; // Size Buffer에 들어온 이름을 _str에 넘겨준다
}

void SaveResRef(Ptr<CRes> _Res, FILE* _File)
{
	int i = 0;
	if (nullptr == _Res)
	{
		fwrite(&i, sizeof(int), 1, _File);
	}
	else
	{
		i = 1;
		fwrite(&i, sizeof(i), 1, _File);
		//shader가 있으면 키값, 경로, 텍스쳐
		SaveWString(_Res->GetKey(), _File);
		SaveWString(_Res->GetRelativePath(), _File);

	}
}
