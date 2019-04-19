#include "DXUT.h"
#include "CBullet.h"

#include "CRigidbody2D.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"
#include "CTransform2D.h"

#include "CLifeMacine.h"

CBullet::CBullet(lp_gameobject owner_obj)
	:CBaseComponent(owner_obj)
{
}


CBullet::~CBullet()
{
}

void CBullet::Init()
{
}

void CBullet::Update()
{
	m_pRigidbody->AddForce(m_vDir, m_fSpeed);

	if (m_fDeathTime <= GetNowTime())
	{
		GO.Kill();
	}
}

void CBullet::Destroy()
{
}

void CBullet::Setup(Vector3 position, Vector2 direction, const resource_id& id,
	float speed, float damage, int attack_count)
{
	m_vDir = direction;

	m_fSpeed = speed;
	m_fDamage = damage;

	GO.AC(CTransform2D)->SetPosition(position);
	GO.AC(CSpriteRenderer)->Set(id);
	m_pRigidbody = GO.AC(CRigidbody2D);
	m_pRigidbody->SetUseGravity(false);
	m_pRigidbody->SetHorizontalAttenuation(0.95);

	m_pCollider = GO.AC(CCollider2D);
	m_pCollider->Setup();
	m_pCollider->SetIsTrigger(true);
	m_pCollider->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });

	m_fDeathTime = GetNowTime() + 2000;
	m_iAttackCount = attack_count;

	GO.GC(CSpriteRenderer)->Flip(direction.x > 0 ? 1 : -1);
	GO.GC(CSpriteRenderer)->SetRenderLayer(15);
}

void CBullet::OnCollision(CCollider2D * other)
{
	if (other->GetGameObject()->GetObjectID() == "ENEMY")
	{
		CLifeMachine* life = other->GetGameObject()->GC(CLifeMachine);

		if (life)
		{
			if (--m_iAttackCount <= 0)
				GO.Kill();

			life->OnAttack(m_fDamage);
			if (m_fSoundTime <= GetNowTime()) {
				SOUND.DuplicatePlay("HIT");
				m_fSoundTime = GetNowTime() + 200;
			}
			return;
		}
	}

	if (other->GetGameObject()->GetObjectID() == "BOSS")
	{
		CLifeMachine* life = other->GetGameObject()->GC(CLifeMachine);

		if (life)
		{
			if (--m_iAttackCount <= 0)
				GO.Kill();

			life->OnAttack(m_fDamage);
			if (m_fSoundTime <= GetNowTime()) {
				SOUND.DuplicatePlay("FIRE_BALL");
				m_fSoundTime = GetNowTime() + 200;
			}
			return;
		}
	}
}
