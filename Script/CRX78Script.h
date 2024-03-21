#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"

class CRX78Script :
    public CScript
{
private:
	int		Hp;//ĳ���� HP 100%
	int		MaxHp;
	int		MpUse;	// MP�Ҹ� 13 /15 /17
	int		Defense; 	//ĳ���� ���� 100%
	float	Speed;
	int		Lv;	// ���� ���� 1/3/5
	float	CastingTime;	// �����ð� 0.3
	float	CoolTime;	// ��Ÿ�� 2.1
	float	DurationTime; //���ӽð� 7��
	int		Damage;//���� ���ݷ� 1286% / 1406 / 1523 / 1640 /1760/
	float	ExplodeRange;//���� ���� 169%
	float	count;
	bool	b_dir;
	bool	isExplode;
public:
    virtual void begin() override;
    virtual void tick() override; 

    virtual void BeginOverlap(CCollider2D* _Other) override;
	virtual void OnOverlap(CCollider2D* _Other) override;
	virtual void EndOverlap(CCollider2D* _Other) override;
public:
	void SetHP(int _PlayerHP) 
	{
		MaxHp = _PlayerHP;
		Hp = MaxHp;
	}
	void SetDefense(int PlayerDefense)
	{
		Defense = PlayerDefense;
	}
	void SetLevel(int PlayerLevel)
	{
		Lv = PlayerLevel;
	}
	void SetDamage(int PlayerMAttack)
	{
		//���� ���ݷ� 1286% / 1406 / 1523
		if(0<Lv&&Lv<3)
			Damage = PlayerMAttack * 12.86f;
		else if(2<Lv&&Lv<5)
			Damage = PlayerMAttack * 14.06f;
		else if (4 < Lv)
			Damage = PlayerMAttack * 15.23f;
	}
	int GetDamage() { return Damage; }
	CLONE(CRX78Script)
public:
    CRX78Script();
    ~CRX78Script();
};

