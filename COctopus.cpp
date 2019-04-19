#include "DXUT.h"
#include "COctopus.h"

#include "CSpriteRenderer.h"
#include "CTransform2D.h"
#include "CRigidbody2D.h"
#include "CCollider2D.h"
#include "CStateMachine.h"
#include "CLifeMacine.h"

#include "CCoconut.h"
#include "CTentacle.h"
#include "CGageBar.h"

COctopus::COctopus(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj, "BOSS")
{

}


COctopus::~COctopus()
{

}

void COctopus::Init()
{
	m_pRigidbody = GO.AC(CRigidbody2D);
	m_pSpriteRenderer = GO.AC(CSpriteRenderer);
	m_pLifeMachine = GO.AC(CLifeMachine);

	GO.AC(CTransform2D);
	m_pCollider2D = GO.AC(CCollider2D);
	m_pCollider2D->Setup(250, 220);
	m_pCollider2D->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
	m_pCollider2D->SetIsStaticCollider(true);

	SetTexturesByState();
	SetupStateMachine();

	m_pSpriteRenderer->SetRenderLayer(1);
	
	m_pRigidbody->InterlockToCollider();
	m_pRigidbody->SetMess(10.0f);
	m_pRigidbody->SetUseGravity(true);
	m_pRigidbody->SetBouncePower(0.5f);
}

void COctopus::Update()
{
	if (INPUT.GetKeyDown(VK_F2))
	{
		m_pLifeMachine->OnAttack(10);
	}
}

void COctopus::Destroy()
{
}

void COctopus::Setup(float x, float y, float hp)
{
	m_pLifeMachine->Setup(hp);
	GO.GetTransform()->SetPosition(x, y);

	m_vGageBarPos = Vector3(screen_width / 2.f, 50, 0);
	m_pLifeMachine->SetInvincible(true);
}

void COctopus::SetTexturesByState()
{
	m_pTexturesByState[(INT)OCTOPUS_STATE::ATTACK1] = RESOURCE.FindTexture("OCTOPUS_A1");
	m_pTexturesByState[(INT)OCTOPUS_STATE::ATTACK2] = RESOURCE.FindTexture("OCTOPUS_A2");
	m_pTexturesByState[(INT)OCTOPUS_STATE::DEATH]	= RESOURCE.FindTexture("OCTOPUS_D");
}

void COctopus::SetupStateMachine()
{
	m_pStateMachine = GO.AC(CStateMachine);

	m_pStateMachine->AddState(OCTOPUS_STATE::DETEXTION, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)OCTOPUS_STATE::ATTACK1]);
		m_pSpriteRenderer->SetFrame(0, 0, 200);
		m_fDetectionTime = 0;
	}, [&]() {


		if (m_fDetectionTime == 0 && CAMERA.GetPosition().x >= 2500)
		{
			m_fDetectionTime = timeGetTime() + 2000;
			CAMERA.SetTargetTransform(GO.GetTransform());

			SOUND.Stop("STAGE1_BG");
			SOUND.Play("BOSS", TRUE);
		}

		if (m_fDetectionTime != 0 && m_fDetectionTime <= timeGetTime())
		{
			m_pStateMachine->ChangeState(OCTOPUS_STATE::ATTACK1);

			auto gb = Instantiate(CGageBar);
			gb->Setup(&GO, &m_vGageBarPos, Vector3(0, 0, 0), "BIG_GAGE_BAR");
			gb->SetRenderType(D3DXSPRITE_ALPHABLEND);
			gb->SetRenderLayer(13);

			m_pLifeMachine->SetInvincible(false);
		}
	}, [&]() {
	});

	m_pStateMachine->AddState(OCTOPUS_STATE::ATTACK1, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)OCTOPUS_STATE::ATTACK1]);
		m_pSpriteRenderer->SetFrame(0, 4, 500);
		CAMERA.SetTargetTransform(ENTITY.FindGameObject("PLAYER", "PLAYER")->GetTransform());

		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				Vector3 this_pos = *GO.GetTransform()->GetPosition();

				int random1= GetIntRandom(0, 1);
				int random2 = GetIntRandom(2, 3);

				Vector3 pos[4] = { Vector3(2609.1, 356.035,0),
					Vector3(2685.95, 162.136,0),
					Vector3(3442.69, 162.11,0),
					Vector3(3520.77, 356.093,0) };

				Instantiate(CTentacle)->Setup(pos[random1], -1);
				Instantiate(CTentacle)->Setup(pos[random2], 1);

				m_pStateMachine->ChangeState(OCTOPUS_STATE::ATTACK2);
			}
		}, [&]() {
		});

	m_pStateMachine->AddState(OCTOPUS_STATE::ATTACK2, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)OCTOPUS_STATE::ATTACK2]);
		m_pSpriteRenderer->SetFrame(0, 4, 200);
		m_bIsEndAttack = false;
		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 2 && !m_bIsEndAttack)
			{
				Vector3 pos = *GO.GetTransform()->GetPosition();

				int random = GetIntRandom(-1, 1);
				Vector2 dir1 = Vector2(1, 0.25 * random);
				Vector2 dir2 = Vector2(-1, -0.25 * random);

				Instantiate(CCoconut)->Setup(pos + Vector3(100, 0, 0), dir1);
				Instantiate(CCoconut)->Setup(pos + Vector3(-100, 0, 0), dir2);
				m_bIsEndAttack = true;
			}

			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				m_pStateMachine->ChangeState(OCTOPUS_STATE::ATTACK1);
			}

		}, [&]() {
		});

	m_pStateMachine->AddState((INT)OCTOPUS_STATE::DEATH, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)OCTOPUS_STATE::DEATH]);
		m_pSpriteRenderer->SetFrame(0, 4, 200);
		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				GO.Kill();
			}
		}, [&]() {

			SOUND.Stop("BOSS");
			SOUND.Play("STAGE1_BG", TRUE);
		});

	m_pStateMachine->ChangeState((INT)OCTOPUS_STATE::DETEXTION);

	m_pStateMachine->AddNode(&m_pLifeMachine->m_bIsDie, true, OCTOPUS_STATE::ATTACK1, OCTOPUS_STATE::DEATH);
	m_pStateMachine->AddNode(&m_pLifeMachine->m_bIsDie, true, OCTOPUS_STATE::ATTACK2, OCTOPUS_STATE::DEATH);
}

void COctopus::OnCollision(CCollider2D * other)
{

}
