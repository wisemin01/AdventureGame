#include "DXUT.h"
#include "CPianus.h"

#include "CSpriteRenderer.h"
#include "CTransform2D.h"
#include "CRigidbody2D.h"
#include "CCollider2D.h"
#include "CStateMachine.h"
#include "CLifeMacine.h"

#include "CBoneFish.h"
#include "CLaser.h"
#include "CGageBar.h"

CPianus::CPianus(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj, "BOSS")
{

}


CPianus::~CPianus()
{
}

void CPianus::Init()
{
	m_pRigidbody = GO.AC(CRigidbody2D);
	m_pSpriteRenderer = GO.AC(CSpriteRenderer);
	m_pLifeMachine = GO.AC(CLifeMachine);

	GO.AC(CTransform2D);
	m_pCollider2D = GO.AC(CCollider2D);
	m_pCollider2D->Setup(250, 400);
	m_pCollider2D->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
	m_pCollider2D->SetIsStaticCollider(true);

	SetTexturesByState();
	SetupStateMachine();

	m_pSpriteRenderer->SetRenderLayer(1);

	m_pRigidbody->InterlockToCollider();
	m_pRigidbody->SetMess(10.0f);
	m_pRigidbody->SetUseGravity(false);
	m_pRigidbody->SetBouncePower(0.5f);
}

void CPianus::Update()
{
	if (INPUT.GetKeyDown(VK_F2))
	{
		m_pLifeMachine->OnAttack(10);
	}
}

void CPianus::Destroy()
{
}

void CPianus::Setup(float x, float y, float hp)
{
	m_pLifeMachine->Setup(hp);
	GO.GetTransform()->SetPosition(x, y);

	m_vGageBarPos = Vector3(screen_width / 2.f, 50, 0);
	m_pLifeMachine->SetInvincible(true);
}

void CPianus::SetTexturesByState()
{
	m_pTexturesByState[(INT)PIANUS_STATE::IDLE] = RESOURCE.FindTexture("PIANUS_I");
	m_pTexturesByState[(INT)PIANUS_STATE::ATTACK] = RESOURCE.FindTexture("PIANUS_A");
	m_pTexturesByState[(INT)PIANUS_STATE::DEATH] = RESOURCE.FindTexture("PIANUS_D");
}

void CPianus::SetupStateMachine()
{
	m_pStateMachine = GO.AC(CStateMachine);

	m_pStateMachine->AddState(PIANUS_STATE::DETEXTION, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PIANUS_STATE::IDLE]);
		m_pSpriteRenderer->SetFrame(0, 0, 200);
		m_fDetectionTime = 0;
		}, [&]() {

			if (m_fDetectionTime == 0 && CAMERA.GetPosition().x >= 6124)
			{
				m_fDetectionTime = timeGetTime() + 2000;
				CAMERA.SetTargetTransform(GO.GetTransform());

				SOUND.Stop("STAGE1_BG");
				SOUND.Play("BOSS", TRUE);
			}

			if (m_fDetectionTime != 0 && m_fDetectionTime <= timeGetTime())
			{
				m_pStateMachine->ChangeState(PIANUS_STATE::IDLE);

				auto gb = Instantiate(CGageBar);
				gb->Setup(&GO, &m_vGageBarPos, Vector3(0, 0, 0), "BIG_GAGE_BAR");
				gb->SetRenderType(D3DXSPRITE_ALPHABLEND);
				gb->SetRenderLayer(13);

				m_pLifeMachine->SetInvincible(false);
			}
		}, [&]() {
		});

	m_pStateMachine->AddState(PIANUS_STATE::IDLE, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PIANUS_STATE::IDLE]);
		m_pSpriteRenderer->SetFrame(0, 4, 200);
		CAMERA.SetTargetTransform(ENTITY.FindGameObject("PLAYER", "PLAYER")->GetTransform());
		m_fAttackDelay = GetNowTime() + GetIntRandom(2000, 3000);
		}, [&]() {

			if (m_fAttackDelay < GetNowTime())
			{
				m_pStateMachine->ChangeState(PIANUS_STATE::ATTACK);
			}

		}, [&]() {
		});

	m_pStateMachine->AddState(PIANUS_STATE::ATTACK, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PIANUS_STATE::ATTACK]);
		m_pSpriteRenderer->SetFrame(0, 4, 300);
		m_bIsEndAttack = false;

		m_iAttackType = GetIntRandom(1, 2);
		}, [&]() {
			switch (m_iAttackType)
			{
			case 1:
				Attack1();
				break;
			case 2:
				Attack2();
				break;
			}
		}, [&]() {
		});

	m_pStateMachine->AddState((INT)PIANUS_STATE::DEATH, [&]() {
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PIANUS_STATE::DEATH]);
		m_pSpriteRenderer->SetFrame(0, 4, 800);
		}, [&]() {
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				GO.Kill();
			}
		}, [&]() {

			SOUND.Stop("BOSS");
			SOUND.Play("CLEAR", FALSE);
		});

	m_pStateMachine->ChangeState((INT)PIANUS_STATE::DETEXTION);

	m_pStateMachine->AddNode(&m_pLifeMachine->m_bIsDie, true, PIANUS_STATE::IDLE, PIANUS_STATE::DEATH);
	m_pStateMachine->AddNode(&m_pLifeMachine->m_bIsDie, true, PIANUS_STATE::ATTACK, PIANUS_STATE::DEATH);
}

void CPianus::OnCollision(CCollider2D * other)
{

}

void CPianus::Attack1()
{
	if (!m_bIsEndAttack && m_pSpriteRenderer->GetFrame()->current == 2) {
		Instantiate(CLaser)->Setup(*GO.GetTransform()->GetPosition() + Vector3(-400, 100, 0), 0.3);
		console_log("shoot laser");
		m_pSpriteRenderer->SetFrame(2, 4, 650);
		m_bIsEndAttack = true;
	}

	if (m_pSpriteRenderer->GetFrame()->current == 4)
	{
		m_pStateMachine->ChangeState(PIANUS_STATE::IDLE);
	}
}

void CPianus::Attack2()
{
	if (!m_bIsEndAttack && m_pSpriteRenderer->GetFrame()->current == 2) {
		
		m_pSpriteRenderer->SetFrame(2, 4, 200);
		m_bIsEndAttack = true;
		Instantiate(CBoneFish)->Setup(*GO.GetTransform()->GetPosition()
			+ Vector3(GetIntRandom(-500, 0), -300, 0), "BONE_FISH", 7, 2.5);
	}

	if (m_pSpriteRenderer->GetFrame()->current == 4)
	{
		m_pStateMachine->ChangeState(PIANUS_STATE::IDLE);
	}
}
