#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CEZ8Script :
    public CScript
{
private:
	int		Hp;//캐릭터 HP 300%
	int		MaxHp;
	int		MpUse;	// MP소모량 72 /79 /86
	int		Defense; 	//캐릭터 방어력 300%
	float	Speed;
	int		Lv;	// 습득 레벨 1/3/5
	float	CastingTime;	// 시전시간 0.3
	float	CoolTime;	// 쿨타임 5.5
	float	DurationTime; //지속시간 7초
	int		Damage;//마법 공격력 1286% / 1406 / 1523 / 1640 /1760/
	float	ExplodeRange;//폭팔 범위 132%
	float	count;
	bool	b_dir;
	bool	isexplode;
	float	time = 0;
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _Other) override;
	virtual void OnOverlap(CCollider2D* _Other) override;
	virtual void EndOverlap(CCollider2D* _Other) override;
public:
	void SetHP(int _PlayerHP)
	{
		MaxHp = _PlayerHP*3;
		Hp = MaxHp;
	}
	void SetDefense(int PlayerDefense)
	{
		Defense = PlayerDefense*3;
	}
	void SetLevel(int PlayerLevel)
	{
		Lv = PlayerLevel;
	}
	void SetDamage(int PlayerMAttack)
	{
		//마법 공격력 4613% / 5039 / 5465
		if (0 < Lv && Lv < 3)
			Damage = PlayerMAttack * 46.13f;
		else if (2 < Lv && Lv < 5)
			Damage = PlayerMAttack * 50.39f;
		else if (4 < Lv)
			Damage = PlayerMAttack * 54.65f;
	}
	int GetDamage() { return Damage; }
	bool IsExplode() { return isexplode; }
    CLONE(CEZ8Script)
public:
    CEZ8Script();
    ~CEZ8Script();
};

