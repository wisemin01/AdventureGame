#include "DXUT.h"
#include "CTargetBullet.h"

#include "CStateMachine.h"

#include "CCollider2D.h"
#include "CRigidbody2D.h"
#include "CSpriteRenderer.h"

CTargetBullet::CTargetBullet(lp_gameobject owner_obj)
	: CBullet(owner_obj)
{
}


CTargetBullet::~CTargetBullet()
{
}

void CTargetBullet::Init()
{
}

void CTargetBullet::Update()
{
	if (m_fDeathTime <= GetNowTime())
		GO.Kill();
}

void CTargetBullet::Destroy()
{
}

void CTargetBullet::Setup(Vector3 position, const resource_id & id, float speed, float damage)
{
	CBullet::Setup(position, Vector2(0, 0), id, speed, damage);
	m_pStateMachine = GO.AC(CStateMachine);

	m_pCollider->FuncClear();

	m_fDeathTime += 2000.f;
		m_pStateMachine->AddState(TARGET_BULLET_STATE::DETECT, [&]() {
			
		m_pCollider->SetRange(700, 600);
		m_pCollider->AddOnCollisionFunc([&](CCollider2D* other) {
			if (other->GetGameObject()->GetObjectID() == "ENEMY" ||
				other->GetGameObject()->GetObjectID() == "BOSS")
			{
				m_pTarget = other->GetGameObject();
			}
			});
				m_fTimeRot = 0.0f;
			
			} ,[&]() {
				if (m_pTarget)
				{
					m_pStateMachine->ChangeState(TARGET_BULLET_STATE::FOLLOW);
				}


				m_fTimeRot += GetDeltaTime() * 1.5f;

				GO.GetTransform()->SetRotation(D3DXToDegree(m_fTimeRot));
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

			GO.GetTransform()->SetRotation(D3DXToDegree(atan2f(m_vDir.y, m_vDir.x)));

			m_pRigidbody->AddForce(m_vDir, m_fSpeed);
		}, [&]() {
		
		});

	m_pStateMachine->ChangeState(TARGET_BULLET_STATE::DETECT);
}
