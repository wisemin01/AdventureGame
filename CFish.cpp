#include "DXUT.h"
#include "CFish.h"

#include "CSpriteRenderer.h"
#include "CTransform2D.h"
#include "CRigidbody2D.h"
#include "CCollider2D.h"
#include "CStateMachine.h"
#include "CLifeMacine.h"

#include "CGageBar.h"

#define GAP (10)

CFish::CFish(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj, "ENEMY")
{

}


CFish::~CFish()
{
}

void CFish::Init()
{
	m_pRigidbody = GO.AC(CRigidbody2D);
	m_pSpriteRenderer = GO.AC(CSpriteRenderer);
	m_pLifeMachine = GO.AC(CLifeMachine);

	GO.AC(CTransform2D)->SetScale(0.7, 0.7);
	GO.AC(CCollider2D)->Setup(-60, -21, 60, 71);
	GO.GC(CCollider2D)->AddOnCollisionFunc([&](CCollider2D* other) {OnCollision(other); });

	SetTexturesByState();
	SetupStateMachine();

	m_pSpriteRenderer->SetRenderLayer(1);
	m_pLifeMachine->Setup(3);
}

void CFish::Update()
{
	m_pSpriteRenderer->Flip(m_iXDir);

	if (GO.GetTransform()->GetPosition()->y >= screen_width + 20)
	{
		GO.Kill();
	}
}

void CFish::Destroy()
{
}

void CFish::SetTexturesByState()
{
	m_pTexturesByState[(INT)FISH_STATE::WALK]	= RESOURCE.FindTexture("FISH_W");
	m_pTexturesByState[(INT)FISH_STATE::ATTACK] = RESOURCE.FindTexture("FISH_A");
	m_pTexturesByState[(INT)FISH_STATE::DEATH]	= RESOURCE.FindTexture("FISH_D");
}

void CFish::SetupStateMachine()
{
	m_pStateMachine = GO.AC(CStateMachine);

	m_pStateMachine->AddState((INT)FISH_STATE::WALK, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)FISH_STATE::WALK]);
		m_pSpriteRenderer->SetFrame(0, 4, 200);

		m_fAttackTime = GetNowTime() + GetIntRandom(3000, 4500);
		}, [&]() {
			Vector3 pos = *GO.GetTransform()->GetPosition();

			if (m_iXDir > 0) {
				if (pos.x >= m_fEndRight)
					m_iXDir = -1;
			}
			else
			{
				if (pos.x <= m_fEndLeft)
					m_iXDir = 1;
			}

			m_pRigidbody->AddForce(m_fSpeed * m_iXDir, -CRigidbody2D::GetGravity());

			float depth = m_iYBaseLine - pos.y;

			if (abs(depth) > 5)
				m_pRigidbody->AddForce(0, depth);

			if (m_fAttackTime <= GetNowTime())
			{
				m_bIsAttack = true;
			}

		}, [&]() {
		});

	m_pStateMachine->AddState((INT)FISH_STATE::ATTACK, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)FISH_STATE::ATTACK]);
		m_pSpriteRenderer->SetFrame(0, 4, 120);
		m_bIsUp = true;

		m_pRigidbody->SetVelocity(Vector3(0, 0, 0));
		m_pRigidbody->AddForce(0, -600);
		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				m_pSpriteRenderer->SetFrame(4, 4, 0);
				m_bIsUp = false;
			}

			if (!m_bIsUp && GetGameObject()->GetTransform()->GetPosition()->y >= m_iYBaseLine)
			{
				m_bIsAttack = false;
			}

			m_pRigidbody->AddForce(m_fSpeed * m_iXDir * 4, 0);

		}, [&]() {
		});

	m_pStateMachine->AddState((INT)FISH_STATE::DEATH, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)FISH_STATE::DEATH]);
		GO.GC(CCollider2D)->UnRegister();
		m_pRigidbody->SetVelocity(Vector3(0, 0, 0));
		m_pRigidbody->AddForce(0, -50);
		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				m_pSpriteRenderer->SetFrame(4, 4, 0);
			}
		}, [&]() {
		});

	m_pStateMachine->ChangeState((INT)FISH_STATE::WALK);

	m_pStateMachine->AddNode(&m_bIsAttack, true, (INT)FISH_STATE::WALK, (INT)FISH_STATE::ATTACK);
	m_pStateMachine->AddNode(&m_bIsAttack, false, (INT)FISH_STATE::ATTACK, (INT)FISH_STATE::WALK);

	m_pStateMachine->AddNode(&m_pLifeMachine->m_bIsDie, true, (INT)FISH_STATE::WALK, (INT)FISH_STATE::DEATH);
	m_pStateMachine->AddNode(&m_pLifeMachine->m_bIsDie, true, (INT)FISH_STATE::ATTACK, (INT)FISH_STATE::DEATH);
}

void CFish::OnCollision(CCollider2D * other)
{
	CCollider2D* this_collider = GO.GC(CCollider2D);

	if (other->GetGameObject()->GetObjectTag() == "PLAYER")
	{
		RECT out_rc;

		RECT this_rc = this_collider->GetWorldRange();
		RECT player_rc = other->GetWorldRange();

		Vector3 this_pos = *GO.GetTransform()->GetPosition();
		Vector3 player_pos = *other->m_pGameObject->GetTransform()->GetPosition();

		if (IntersectRect(&out_rc, &this_rc,
			&player_rc))
		{
			int width = out_rc.right - out_rc.left;
			int height = out_rc.bottom - out_rc.top;

			if (width > height)
			{
				if (player_pos.y <= this_pos.y)
				{
					m_pLifeMachine->OnAttack(5);
				}

				CRigidbody2D* rigidbody = other->GetGameObject()->GC(CRigidbody2D);

				if (rigidbody)
				{
					rigidbody->SetVelocityY(0.0f);
					rigidbody->AddForce(0, -400);
					SOUND.DuplicatePlay("HIT");
				}
			}
			else
			{
				CLifeMachine* life = other->GetGameObject()->GC(CLifeMachine);

				if (life)
				{
					life->OnAttack(0.5);
				}

				CRigidbody2D* rigidbody = other->GetGameObject()->GC(CRigidbody2D);

				if (rigidbody)
				{
					rigidbody->AddForce(transform::GetDirection(other->GetGameObject()->GetTransform(),
						GetGameObject()->GetTransform()), 400);
				}
			}
		}
	}
}

void CFish::Set(float x, float y, float left_end, float right_end)
{
	GO.GetTransform()->SetPosition(x, y);

	m_fEndLeft = left_end;
	m_fEndRight = right_end;

	m_iYBaseLine = y;
	m_iYJumpLine = y - 500;

	Instantiate(CGageBar)->Setup(&GO, GO.GetTransform()->GetPosition(), Vector3(0, -50, 0), "GAGE_BAR");
}