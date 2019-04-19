#include "DXUT.h"
#include "CLifeMacine.h"


CLifeMachine::CLifeMachine(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj)
{
}


CLifeMachine::~CLifeMachine()
{
}

void CLifeMachine::Init()
{
}

void CLifeMachine::Update()
{
	if (m_fCurHp <= 0)
		m_bIsDie = true;
}

void CLifeMachine::Destroy()
{
}

void CLifeMachine::OnAttack(float damage)
{
	if (m_bIsInvincible)
	{
		return;
	}

	m_fCurHp -= damage;
}

void CLifeMachine::OnHeal(float heal)
{
	m_fCurHp += heal;
}
