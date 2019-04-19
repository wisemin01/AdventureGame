#include "DXUT.h"
#include "CLaser.h"

#include "CTransform2D.h"
#include "CSpriteRenderer.h"
#include "CCollider2D.h"
#include "CLifeMacine.h"


CLaser::CLaser(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj)
{
}


CLaser::~CLaser()
{
}

void CLaser::Init()
{

}

void CLaser::Update()
{
	if (!m_bIsAttacking && m_pSpriteRenderer->GetFrame()->current == 3)
	{
		m_bIsAttacking = true;
		m_fDeleteTime = GetNowTime() + 1500;
		GO.GC(CCollider2D)->Register();

		m_pSpriteRenderer->SetFrame(2, 4, 650);
	}

	if (m_bIsAttacking)
	{
		if (m_fDeleteTime <= GetNowTime())
		{
			GO.Kill();
		}
	}
}

void CLaser::Destroy()
{
}

void CLaser::Setup(Vector3 start_pos, float tick_damage)
{
	GO.AC(CTransform2D)->SetPosition(start_pos);
	m_pSpriteRenderer = GO.AC(CSpriteRenderer);
	m_pSpriteRenderer->Set("LASER");
	GO.AC(CCollider2D)->SetRange(340, 136);

	GO.GC(CCollider2D)->AddOnCollisionFunc(COLLISION_FUNC(other)
	{
		if (m_fLaserTick > GetNowTime())
			return;

		m_fLaserTick = GetNowTime() + 100;

		if (other->GetGameObject()->GetObjectID() == "PLAYER")
		{
			CLifeMachine* life = other->GetGameObject()->GC(CLifeMachine);

			if (life)
			{
				life->OnAttack(m_fDamage);
			}
		}
	});
	GO.GC(CCollider2D)->SetIsTrigger(true);
	m_pSpriteRenderer->SetRenderLayer(12);
	m_pSpriteRenderer->SetFrame(0, 4, 200);

	m_fLaserTick = GetNowTime() + 100;
	m_fDeleteTime = 0;
	m_bIsAttacking = false;
	m_fDamage = tick_damage;
}
