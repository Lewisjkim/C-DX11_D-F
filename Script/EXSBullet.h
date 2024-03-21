#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class EXSBullet :
    public CScript
{
private:
	float	Speed;
	float	DurationTime; //지속시간 2초
	int		Damage;
	float	count;
	bool	b_dir;
	float	time = 0;
public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _Other) override;
	virtual void OnOverlap(CCollider2D* _Other) override;
	virtual void EndOverlap(CCollider2D* _Other) override;
public:
	void SetBulletDir(bool dir) { b_dir = dir; }
	void SetDamage(int PlayerMAttack)
	{
		Damage = PlayerMAttack;
	}
	int GetDamage() { return Damage; }
	CLONE(EXSBullet)
public:
	EXSBullet();
	~EXSBullet();
};

