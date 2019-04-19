#include "DXUT.h"
#include "CPlayer.h"

#include "CSpriteRenderer.h"
#include "CTransform2D.h"
#include "CCollider2D.h"
#include "CRigidbody2D.h"

#include "CStateMachine.h"
#include "CLifeMacine.h"

#include "CTargetBullet.h"
#include "CGageBar.h"

#define JUMP_DEPTH 15

//Vector3 CPlayer::g_vReSpawn;

CPlayer::CPlayer(lp_gameobject owner)
	: CBaseComponent(owner, "PLAYER")
{
}

CPlayer::~CPlayer()
{

}

void CPlayer::Init()
{
	GO.AddComponent<CTransform2D>();
	GO.AddComponent<CStateMachine>();
	GO.AddComponent<CLifeMachine>()->Setup(5);

	m_pSpriteRenderer = GO.AddComponent<CSpriteRenderer>();
	m_pSpriteRenderer->SetFrame(0, 4, 200);

	GO.SetObjectTag("PLAYER");

	GO.GetTransform()->SetPosition(screen_width / 2.f, screen_height / 2.f);

	CAMERA.SetTargetTransform(GO.GetTransform());

	SetupTexturesByState();
	SetupCollider();
	SetupRigidbody();
	SetupStateMachine();
}

void CPlayer::Update()
{
	if (INPUT.GetKeyDown(VK_F3))
	{
		ENTITY.QuickSend(eMsgDestroy, &GO);
	}

	if (INPUT.GetKeyDown(VK_F1))
	{
		lp_gameobject_list list = ENTITY.GetObjectList("ENEMY");
		for (auto iter : *list)
		{
			iter->GC(CLifeMachine)->OnAttack(10);
		}
	}

	if (INPUT.GetKeyDown(VK_F6))
	{
		Vector3 out = *GO.GetTransform()->GetPosition();
		console_log(out.x << ", " << out.y);
	}

	Vector3* pos = GO.GetTransform()->GetPosition();

	float min, max;

	std::tie(min, max) = CAMERA.GetMapWidth();

	if (pos->x <= min)
		pos->x = min;

	if (pos->x > max)
		pos->x = max;

	if (pos->y >= screen_height + 20)
	{
		GO.GC(CLifeMachine)->m_bIsDie = true;
	}

	m_bIsOnObject = false;
}

void CPlayer::Destroy()
{
	console_log("CPlayer - Destroy()");

	if (CAMERA.GetTargetTransform() == GO.GetTransform())
		CAMERA.SetTargetTransform(null);
}

void CPlayer::HandleMessage(lp_message msg)
{
	switch (msg->GetType())
	{
	case eMsgDestroy:
		console_log("eMsgDestroy 메시지 받음.");

		ENTITY.SendMessageToAll(eMsgPlayerDeath, &GO);
		return;
	}
}

void CPlayer::OnCollision(CCollider2D * other)
{
	float player_foot_y = GO.GetTransform()->GetPosition()->y + m_fFootHeight * GO.GetTransform()->GetScale()->y;
	float other_top = other->GetWorldRange().top;

	if (player_foot_y <= other_top + JUMP_DEPTH && player_foot_y >= other_top - JUMP_DEPTH)
		m_bIsOnObject = true;
	else if (other->m_pGameObject->GetObjectID() == "WATER")
		m_bIsOnObject = true;
}

void CPlayer::Setup(float x, float y, float size)
{
	GO.GetTransform()->SetPosition(x, y);
	GO.GetTransform()->SetScale(size, size);

	auto gb = Instantiate(CGageBar);
	gb->Setup(&GO, GO.GetTransform()->GetPosition(), Vector3(0, -70, 0), "GAGE_BAR");
}

void CPlayer::SetupStateMachine()
{
	m_pStateMachine = GO.GC(CStateMachine);

	if (!m_pStateMachine)
		return;

	m_pStateMachine->AddState((INT)PLAYER_STATE::IDLE, [&]() {
			// IDLE INIT
			m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PLAYER_STATE::IDLE][(INT)m_eItemState]);
			m_pSpriteRenderer->SetFrame(0, 4, 200);
		}, [&]() {
			// IDLE UPDATE
			JumpCheck();
			AttackCheck();	
			MoveCheck();
			ModeCheck();

		}, [&]() {
			// IDLE DESTROY
		});

	m_pStateMachine->AddState((INT)PLAYER_STATE::WALK, [&]() {
			// WALK INIT
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PLAYER_STATE::WALK][(INT)m_eItemState]);
		m_pSpriteRenderer->SetFrame(0, 4, 120);
		}, [&]() {
			// WALK UPDATE
			JumpCheck();
			AttackCheck();
			MoveCheck();
			ModeCheck();

		}, [&]() {
			// WALK DESTROY
		});

	m_pStateMachine->AddState((INT)PLAYER_STATE::JUMP, [&]() {
			// JUMP INIT
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PLAYER_STATE::JUMP][(INT)m_eItemState]);
		m_pSpriteRenderer->SetFrame(0, 4, 100);
		SOUND.DuplicatePlay("JUMP");
		}, [&]() {
			// JUMP UPDATE
			if (m_pSpriteRenderer->GetFrame()->current == 2)
			{
				m_pSpriteRenderer->SetFrame(2, 2);
			}
			AttackCheck();
			MoveCheck();
			ModeCheck();

		}, [&]() {
			// JUMP DESTROY
			m_pSpriteRenderer->SetFrame(0, 4, 200);
		});

	m_pStateMachine->AddState((INT)PLAYER_STATE::ATTACK, [&]() {
			// ATTACK INIT
		m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PLAYER_STATE::ATTACK][(INT)m_eItemState]);
		m_pSpriteRenderer->SetFrame(0, 4, m_fAttackDelay[(INT)m_eItemState]);

		}, [&]() {
			// ATTACK UPDATE
			if (m_pSpriteRenderer->GetFrame()->current == 4)
			{
				Attack(m_eItemState);
				m_pStateMachine->ChangeState((INT)PLAYER_STATE::IDLE);
			}
		}, [&]() {
			// ATTACK DESTROY
		});

	m_pStateMachine->AddState((INT)PLAYER_STATE::DEATH, [&]() {
		// DEATH INIT
		//m_pSpriteRenderer->SetByTexture(m_pTexturesByState[(INT)PLAYER_STATE::DEATH][(INT)m_iItemState]);
		//m_pSpriteRenderer->SetFrame(0, 4, 50);

		GO.Kill();

		ENTITY.CreateDefault<CPlayer>()->Setup(g_vReSpawn.x, g_vReSpawn.y, 0.45f);

		}, [&]() {
			// DEATH UPDATE

		}, [&]() {
			// DEATH DESTROY
		});

	m_pStateMachine->ChangeState((INT)PLAYER_STATE::IDLE);

	m_pStateMachine->AddNode(&m_bIsOnFrameMove	, true	, (INT)PLAYER_STATE::IDLE, (INT)PLAYER_STATE::WALK);
	m_pStateMachine->AddNode(&m_bIsJumpOnFrame	, true	, (INT)PLAYER_STATE::IDLE, (INT)PLAYER_STATE::JUMP);
	m_pStateMachine->AddNode(&m_bIsAttackOnFrame, true	, (INT)PLAYER_STATE::IDLE, (INT)PLAYER_STATE::ATTACK);
	m_pStateMachine->AddNode(&GO.GC(CLifeMachine)->m_bIsDie, true	, (INT)PLAYER_STATE::IDLE, (INT)PLAYER_STATE::DEATH);

	m_pStateMachine->AddNode(&m_bIsOnFrameMove	, false	, (INT)PLAYER_STATE::WALK, (INT)PLAYER_STATE::IDLE);
	m_pStateMachine->AddNode(&m_bIsJumpOnFrame	, true	, (INT)PLAYER_STATE::WALK, (INT)PLAYER_STATE::JUMP);
	m_pStateMachine->AddNode(&m_bIsAttackOnFrame, true	, (INT)PLAYER_STATE::WALK, (INT)PLAYER_STATE::ATTACK);
	m_pStateMachine->AddNode(&GO.GC(CLifeMachine)->m_bIsDie, true	, (INT)PLAYER_STATE::WALK, (INT)PLAYER_STATE::DEATH);

	m_pStateMachine->AddNode(&m_bIsOnObject		, true	, (INT)PLAYER_STATE::JUMP, (INT)PLAYER_STATE::IDLE);
	m_pStateMachine->AddNode(&m_bIsAttackOnFrame, true	, (INT)PLAYER_STATE::JUMP, (INT)PLAYER_STATE::ATTACK);
	m_pStateMachine->AddNode(&GO.GC(CLifeMachine)->m_bIsDie, true	, (INT)PLAYER_STATE::JUMP, (INT)PLAYER_STATE::DEATH);
}

void CPlayer::SetupCollider()
{
	auto col = GO.AddComponent<CCollider2D>();

	col->SetRange(137, 223);
	col->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
	col->Register();

	m_fFootHeight = col->GetHeight() / 2;
}

void CPlayer::SetupRigidbody()
{
	m_pRigidbody = GO.AddComponent<CRigidbody2D>();
	m_pRigidbody->SetBouncePower(0.6f);
	m_pRigidbody->SetMess(3.0f);

	m_pRigidbody->InterlockToCollider();
}

void CPlayer::SetupTexturesByState()
{
	m_pTexturesByState[(INT)PLAYER_STATE::IDLE][(INT)PLAYER_ITEM_STATE::DEFAULT]		= RESOURCE.FindTexture("PLAYER_DEFAULT_I");
	m_pTexturesByState[(INT)PLAYER_STATE::IDLE][(INT)PLAYER_ITEM_STATE::_3DIRECTION]	= RESOURCE.FindTexture("PLAYER_3DIR_I");
	m_pTexturesByState[(INT)PLAYER_STATE::IDLE][(INT)PLAYER_ITEM_STATE::NUCLEAR]		= RESOURCE.FindTexture("PLAYER_NUCLEUS_I");
	m_pTexturesByState[(INT)PLAYER_STATE::IDLE][(INT)PLAYER_ITEM_STATE::TARGET]			= RESOURCE.FindTexture("PLAYER_TARGET_I");

	m_pTexturesByState[(INT)PLAYER_STATE::WALK][(INT)PLAYER_ITEM_STATE::DEFAULT]		= RESOURCE.FindTexture("PLAYER_DEFAULT_W");
	m_pTexturesByState[(INT)PLAYER_STATE::WALK][(INT)PLAYER_ITEM_STATE::_3DIRECTION]	= RESOURCE.FindTexture("PLAYER_3DIR_W");
	m_pTexturesByState[(INT)PLAYER_STATE::WALK][(INT)PLAYER_ITEM_STATE::NUCLEAR]		= RESOURCE.FindTexture("PLAYER_NUCLEUS_W");
	m_pTexturesByState[(INT)PLAYER_STATE::WALK][(INT)PLAYER_ITEM_STATE::TARGET]			= RESOURCE.FindTexture("PLAYER_TARGET_W");

	m_pTexturesByState[(INT)PLAYER_STATE::JUMP][(INT)PLAYER_ITEM_STATE::DEFAULT]		= RESOURCE.FindTexture("PLAYER_DEFAULT_J");
	m_pTexturesByState[(INT)PLAYER_STATE::JUMP][(INT)PLAYER_ITEM_STATE::_3DIRECTION]	= RESOURCE.FindTexture("PLAYER_3DIR_J");
	m_pTexturesByState[(INT)PLAYER_STATE::JUMP][(INT)PLAYER_ITEM_STATE::NUCLEAR]		= RESOURCE.FindTexture("PLAYER_NUCLEUS_J");
	m_pTexturesByState[(INT)PLAYER_STATE::JUMP][(INT)PLAYER_ITEM_STATE::TARGET]			= RESOURCE.FindTexture("PLAYER_TARGET_J");

	m_pTexturesByState[(INT)PLAYER_STATE::ATTACK][(INT)PLAYER_ITEM_STATE::DEFAULT]		= RESOURCE.FindTexture("PLAYER_DEFAULT_A");
	m_pTexturesByState[(INT)PLAYER_STATE::ATTACK][(INT)PLAYER_ITEM_STATE::_3DIRECTION]	= RESOURCE.FindTexture("PLAYER_3DIR_A");
	m_pTexturesByState[(INT)PLAYER_STATE::ATTACK][(INT)PLAYER_ITEM_STATE::NUCLEAR]		= RESOURCE.FindTexture("PLAYER_NUCLEUS_A");
	m_pTexturesByState[(INT)PLAYER_STATE::ATTACK][(INT)PLAYER_ITEM_STATE::TARGET]		= RESOURCE.FindTexture("PLAYER_TARGET_A");

	m_fAttackDelay[(INT)PLAYER_ITEM_STATE::DEFAULT]		= 50;
	m_fAttackDelay[(INT)PLAYER_ITEM_STATE::_3DIRECTION] = 75;
	m_fAttackDelay[(INT)PLAYER_ITEM_STATE::NUCLEAR]		= 100;
	m_fAttackDelay[(INT)PLAYER_ITEM_STATE::TARGET]		= 125;
}

void CPlayer::JumpCheck()
{
	if (m_bIsOnObject && INPUT.GetKeyDown('C'))
	{
		m_pRigidbody->AddForce(0, -1200);
	
		m_bIsOnObject = false;
		m_bIsJumpOnFrame = true;
	}
	else
		m_bIsJumpOnFrame = false;
}

void CPlayer::AttackCheck()
{
	if (INPUT.GetKeyPress('A'))
	{
		m_bIsAttackOnFrame = true;
	}
	else
		m_bIsAttackOnFrame = false;
}

void CPlayer::MoveCheck()
{
	m_bIsOnFrameMove = false;

	if (INPUT.GetKeyPress(VK_LEFT)) {
		m_pRigidbody->AddForce(-20, 0);
		m_bIsOnFrameMove = true;
		m_pSpriteRenderer->Flip(-1);
	}
	
	if (INPUT.GetKeyPress(VK_RIGHT)) {
		m_pRigidbody->AddForce(20, 0);
		m_bIsOnFrameMove = true;
		m_pSpriteRenderer->Flip(1);
	}
}

void CPlayer::ModeCheck()
{
	if (INPUT.GetKeyDown('X'))
	{
		int current = (int)m_eItemState;
		if (++current >= (INT)PLAYER_ITEM_STATE::END)
		{
			current = (INT)PLAYER_ITEM_STATE::DEFAULT;
		}
		ChangeMode((PLAYER_ITEM_STATE)current);
	}
}

void CPlayer::ChangeMode(PLAYER_ITEM_STATE item_state)
{
	m_eItemState = item_state;

	m_pSpriteRenderer->SetByTexture(m_pTexturesByState[m_pStateMachine->GetNowState()][(INT)m_eItemState]);
}

void CPlayer::Attack(PLAYER_ITEM_STATE item_state)
{
	switch (item_state)
	{
		case PLAYER_ITEM_STATE::DEFAULT:
			Instantiate(CBullet)->Setup(*GO.GetTransform()->GetPosition(), Vector2(m_pSpriteRenderer->GetFlip(), 0), "BULLET", 30, 3);
			break;
		case PLAYER_ITEM_STATE::_3DIRECTION:
			Instantiate(CBullet)->Setup(*GO.GetTransform()->GetPosition(), Vector2(m_pSpriteRenderer->GetFlip(), 0.15), "BULLET", 30, 1);
			Instantiate(CBullet)->Setup(*GO.GetTransform()->GetPosition(), Vector2(m_pSpriteRenderer->GetFlip(), 0), "BULLET", 30, 1);
			Instantiate(CBullet)->Setup(*GO.GetTransform()->GetPosition(), Vector2(m_pSpriteRenderer->GetFlip(), -0.15), "BULLET", 30, 1);
			break;
		case PLAYER_ITEM_STATE::NUCLEAR:
			Instantiate(CBullet)->Setup(*GO.GetTransform()->GetPosition(), Vector2(m_pSpriteRenderer->GetFlip(), 0), "FIRE_BULLET", 10, 1, 15);

			break;
		case PLAYER_ITEM_STATE::TARGET:
			Instantiate(CTargetBullet)->Setup(*GO.GetTransform()->GetPosition(), "DARK_BULLET", 30, 1);
			break;
	}
}
