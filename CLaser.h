#pragma once
#include "CBaseComponent.h"

class CSpriteRenderer;
class CLaser :
	public CBaseComponent
{
private:
	CSpriteRenderer * m_pSpriteRenderer;

	float m_fDamage;
	float m_fDeleteTime;
	float m_fLaserTick;

	bool m_bIsAttacking;
public:
	CLaser(lp_gameobject owner_obj);
	virtual ~CLaser();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(Vector3 start_pos, float tick_damage);
};

