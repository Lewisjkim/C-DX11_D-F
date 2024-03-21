#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CDRunnerScript :
    public CScript
{
private:
	int		Hp;//캐릭터 HP 100%
	int		MaxHp;
	int		MpUse;	// MP소모량 13 /15 /17
	int		Defense; 	//캐릭터 방어력 100%
	float	Speed;
	int		Lv;	// 습득 레벨 1/3/5
	float	CastingTime;	// 시전시간 0.3
	float	CoolTime;	// 쿨타임 2.1
	float	DurationTime; //지속시간 7초
	int		Damage;//마법 공격력 1286% / 1406 / 1523 / 1640 /1760/
	float	ExplodeRange;//폭팔 범위 169%
	float	count;
	bool	b_dir;
	bool	isExplode;
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
	virtual void OnOverlap(CCollider2D* _Other) override;
	virtual void EndOverlap(CCollider2D* _Other) override;
	void SetLevel(int PlayerLevel)
	{
		Lv = PlayerLevel;
	}
	void SetDamage(int PlayerMAttack)
	{
		if (0 < Lv && Lv < 3)
			Damage = PlayerMAttack * 60.50f;
		else if (2 < Lv && Lv < 5)
			Damage = PlayerMAttack * 67.84f;
		else if (4 < Lv)
			Damage = PlayerMAttack * 74.16f;
	}
	int GetDamage() { return Damage; }
    CLONE(CDRunnerScript)
public:
    CDRunnerScript();
    ~CDRunnerScript();
};

