#pragma once
#include "CBaseComponent.h"


class CLifeMachine :
	public CBaseComponent
{
protected:
	float m_fMaxHp = 0;
	float m_fCurHp = 0;

	bool m_bIsInvincible = false;

public:
	CLifeMachine(lp_gameobject owner_obj);
	virtual ~CLifeMachine();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(float max_hp, float cur_hp = 0.0f)
	{
		m_fMaxHp = max_hp;

		if (cur_hp == 0.0f)
			m_fCurHp = max_hp;
		else
			m_fCurHp = cur_hp;
	}

	float GetHP() { return m_fCurHp; }
	float GetMaxHP() { return m_fMaxHp; }

	void OnAttack(float damage);
	void OnHeal(float heal);

	void SetInvincible(bool value) { m_bIsInvincible = value; }

public:
	bool m_bIsDie = false;
};

