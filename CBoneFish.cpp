#include "DXUT.h"
#include "CBoneFish.h"

#include "CStateMachine.h"

#include "CCollider2D.h"
#include "CRigidbody2D.h"
#include "CSpriteRenderer.h"

#include "CLifeMacine.h"

CBoneFish::CBoneFish(lp_gameobject owner_obj)
	: CTargetBullet(owner_obj)
{
}


CBoneFish::~CBoneFish()
{
}

void CBoneFish::Init()
{
}

void CBoneFish::Update()
{
	if (m_fDeathTime <= GetNowTime())
		GO.Kill();
}

void CBoneFish::Destroy()
{
}

void CBoneFish::Setup(Vector3 position, const resource_id & id, float speed, float damage)
{
	CBullet::Setup(position, Vector2(0, 0), id, speed, damage);
	m_pStateMachine = GO.AC(CStateMachine);
	m_pSpriteRenderer = GO.GC(CSpriteRenderer);

	m_pCollider->FuncClear();

	m_fDeathTime += 2000.0f;
	m_pStateMachine->AddState(TARGET_BULLET_STATE::DETECT, [&]() {

		m_pCollider->SetRange(800, 700);
		m_pCollider->AddOnCollisionFunc([&](CCollider2D* other) {
			if (other->GetGameObject()->GetObjectID() == "PLAYER")
			{
				m_pTarget = other->GetGameObject();
			}
			});
		}, [&]() {
			if (m_pTarget)
			{
				m_pStateMachine->ChangeState(TARGET_BULLET_STATE::FOLLOW);
			}

		}, [&]() {

		});
	m_pStateMachine->AddState(TARGET_BULLET_STATE::FOLLOW, [&]() {
		m_vDir = Vector2(transform::GetDirection(GO.GetTransform(), m_pTarget->GetTransform()));
		m_pCollider->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
		m_pCollider->SetRangeAsImage();

		}, [&]() {

			if (!m_pTarget->IsLive())
				GO.Kill();

			m_vDir = Vector2(transform::GetDirection(m_pTarget->GetTransform(), GO.GetTransform()));

			if (m_vDir.x >= 0)
				m_pSpriteRenderer->Flip(-1);
			else
				m_pSpriteRenderer->Flip(1);

			m_pRigidbody->AddForce(m_vDir, m_fSpeed);
		}, [&]() {

		});
	m_pStateMachine->AddState(TARGET_BULLET_STATE::DEATH, [&]() {
		GO.Kill();
		}, [&]() {
		}, [&]() {
		});
	m_pStateMachine->ChangeState(TARGET_BULLET_STATE::DETECT);
}

void CBoneFish::OnCollision(CCollider2D * other)
{
	if (other->GetGameObject()->GetObjectID() == "PLAYER") {
		CLifeMachine* life = other->GetGameObject()->GC(CLifeMachine);

		if (life)
		{
			life->OnAttack(m_fDamage);
			GO.Kill();
		}
	}
}
