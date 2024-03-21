#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CEXSScript :
    public CScript
{
private:
	int		Hp;//ĳ���� HP 300%
	int		MaxHp;
	int		MpUse;	// MP�Ҹ� 72 /79 /86
	int		Defense; 	//ĳ���� ���� 300%
	float	Speed;
	int		Lv;	// ���� ���� 1/3/5
	float	CastingTime;	// �����ð� 0.3
	float	CoolTime;	// ��Ÿ�� 3
	float	DurationTime; //���ӽð� 6��
	int		Damage;//���� ���ݷ� 1286% / 1406 / 1523 / 1640 /1760/
	float	ExplodeRange;//���� ���� 132%
	float	leftcount;
	float	rightcount;
	bool	b_dir;
	bool	bullet_dir;
	bool	isexplode;
	float	time = 0;
	float	lresttime;
	float	rresttime;
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _Other) override;
	virtual void OnOverlap(CCollider2D* _Other) override;
	virtual void EndOverlap(CCollider2D* _Other) override;
public:
	//void SetBulletDir(bool dir) { bullet_dir = dir; }
	bool GetBulletDir() {return bullet_dir;	}
	void SetHP(int _PlayerHP)
	{
		MaxHp = _PlayerHP * 3;
		Hp = MaxHp;
	}
	void SetDefense(int PlayerDefense)
	{
		Defense = PlayerDefense * 3;
	}
	void SetLevel(int PlayerLevel)
	{
		Lv = PlayerLevel;
	}
	void SetDamage(int PlayerMAttack)
	{
		//���� ���ݷ� 225% / 246 / 267
		if (0 < Lv && Lv < 3)
			Damage = PlayerMAttack * 2.25f;
		else if (2 < Lv && Lv < 5)
			Damage = PlayerMAttack * 2.46f;
		else if (4 < Lv)
			Damage = PlayerMAttack * 2.67f;
	}
	int GetDamage() { return Damage; }
	bool IsExplode() { return isexplode; }
    CLONE(CEXSScript)
public:
    CEXSScript();
    ~CEXSScript();
};

