#include "DXUT.h"
#include "CConch.h"

#include "CTransform2D.h"
#include "CRigidbody2D.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"

#include "CStateMachine.h"
#include "CLifeMacine.h"

#include "CGageBar.h"

#define GAP (10)

CConch::CConch(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj, "ENEMY")
{

}


CConch::~CConch()
{
}

void CConch::Init()
{
	GO.AC(CTransform2D);
	GO.AC(CCollider2D)->Setup(-60, -21, 60, 71);
	m_pLifeMacine = GO.AC(CLifeMachine);
	m_pLifeMacine->Setup(3);
	m_pRigidbody = GO.AC(CRigidbody2D);
	m_pSpriteRenderer = GO.AC(CSpriteRenderer);

	SetTexturesByState();
	SetupStateMachine();

	GO.GC(CCollider2D)->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });

	m_pRigidbody->SetMess(3.0f);
	m_pRigidbody->SetBouncePower(0.6f);

	GO.GC(CTransform2D)->SetScale(0.7, 0.7, 0);
}

void CConch::Update()
{
	m_pSpriteRenderer->Flip(-m_iXDir);

	if (GO.GetTransform()->GetPosition()->y >= screen_width + 20)
	{
		GO.Kill();
	}
}

void CConch::Destroy()
{
}

void CConch::Set(float x, float y)
{
	GO.GetTransform()->SetPosition(x, y);

	Instantiate(CGageBar)->Setup(&GO, GO.GetTransform()->GetPosition(), Vector3(0, -50, 0), "GAGE_BAR");
}

void CConch::SetTexturesByState()
{
	m_pTexturesByState[(INT)CONCH_STATE::WALK]		= RESOURCE.FindTexture("CONCH_W");
	m_pTexturesByState[(INT)CONCH_STATE::ATTACK]	= RESOURCE.FindTexture("CONCH_A");
	m_pTexturesByState[(INT)CONCH_STATE::DEATH]		= RESOURCE.FindTexture("CONCH_D");
}

void CConch::SetupStateMachine()
{
	m_pStateMachine = GO.AC(CStateMachine);

	m_pStateMachine->AddState((INT)CONCH_STATE::WALK, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)CONCH_STATE::WALK]);
		m_pSpriteRenderer->SetFrame(0, 4, 200);
		}, [&]() {
			if (m_iXDir > 0) {
				if (GO.GetTransform()->GetPosition()->x >= m_fEndRight)
					m_iXDir = -1;
			}
			else
			{
				if (GO.GetTransform()->GetPosition()->x <= m_fEndLeft)
					m_iXDir = 1;
			}

			m_pRigidbody->AddForce(m_fSpeed * m_iXDir, 0);
		}, [&]() {
		});

	m_pStateMachine->AddState((INT)CONCH_STATE::ATTACK, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)CONCH_STATE::ATTACK]);
		}, [&]() {
		}, [&]() {
		});

	m_pStateMachine->AddState((INT)CONCH_STATE::DEATH, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)CONCH_STATE::DEATH]);
		m_pSpriteRenderer->SetFrame(0, 4, 200);
		GO.GC(CCollider2D)->UnRegister();
		m_pRigidbody->SetVelocity(Vector3(0, 0, 0));
		m_pRigidbody->SetUseGravity(false);
		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				GO.Kill();
			}
		}, [&]() {
		});

	m_pStateMachine->ChangeState((INT)CONCH_STATE::WALK);

	m_pStateMachine->AddNode(&m_pLifeMacine->m_bIsDie, true, (INT)CONCH_STATE::WALK, (INT)CONCH_STATE::DEATH);
}

void CConch::OnCollision(CCollider2D * other)
{
	CCollider2D* this_collider = GO.GC(CCollider2D);

	if (other->GetGameObject()->GetObjectID() == "OBSTACLE")
	{
		RECT out_rc;

		RECT this_rc = this_collider->GetWorldRange();
		RECT obstacle_rc = other->GetWorldRange();

		Vector3 this_pos = *GO.GetTransform()->GetPosition();
		Vector3 obstacle_pos = *other->m_pGameObject->GetTransform()->GetPosition();

		if (IntersectRect(&out_rc, &this_rc,
			&obstacle_rc))
		{
			int width = out_rc.right - out_rc.left;
			int height = out_rc.bottom - out_rc.top;

			if (width > height)
			{
				// Y 충돌
				m_fEndLeft = obstacle_rc.left + GAP;
				m_fEndRight = obstacle_rc.right - GAP;
			}
			else
			{
				// X 충돌
			
				if (obstacle_pos.x <= this_pos.x)
					m_iXDir = 1;
				else
					m_iXDir = -1;
			}
		}
	}
	else if(other->GetGameObject()->GetObjectTag() == "PLAYER")
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
					m_pLifeMacine->OnAttack(5);
					SOUND.DuplicatePlay("HIT");
				}

				CRigidbody2D* rigidbody = other->GetGameObject()->GC(CRigidbody2D);

				if (rigidbody)
				{
					rigidbody->SetVelocityY(0.0f);
					rigidbody->AddForce(0, -400);
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