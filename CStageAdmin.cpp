#include "DXUT.h"
#include "CStageAdmin.h"

#include "CScrollBackground.h"
#include "CSlowMoveBackground.h"

#include "CTransform2D.h"
#include "CWaveRenderer.h"
#include "CCollider2D.h"
#include "CStateMachine.h"

#include "CPlayer.h"
#include "CObstacle.h"
#include "CConch.h"
#include "COctopus.h"
#include "CFish.h"
#include "CPianus.h"

Vector3 CPlayer::g_vReSpawn;

CStageAdmin::CStageAdmin(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj,"ADMIN")
{
}


CStageAdmin::~CStageAdmin()
{
}

void CStageAdmin::Init()
{
	//GO.SetObjectTag("STAGE_ADMIN");
	m_pStateMachine = GO.AC(CStateMachine);
}

void CStageAdmin::Update()
{
}

void CStageAdmin::Destroy()
{
}

void CStageAdmin::StartStage1()
{
	StateSetup1();

	m_eGameProgress = GAME_PROGRESS::STAGE1_FIRST;
	m_pStateMachine->ChangeState(m_eGameProgress);
}

void CStageAdmin::StartStage2()
{
	StateSetup2();

	m_eGameProgress = GAME_PROGRESS::STAGE2_FIRST;
	m_pStateMachine->ChangeState(m_eGameProgress);
}

void CStageAdmin::StateSetup1()
{
	// ==============================================
	// SETUP : STAGE1_FIRST
	// ==============================================

	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE1_FIRST, [&]() {

		CAMERA.SetMapWidth(0, 2500);
		SOUND.Play("STAGE1_BG", TRUE);
		CPlayer::SetReSpawnPos(Vector3(100, 270, 0));

		Instantiate(CScrollBackground)->Set("BACKGROUND1", 100);
		Instantiate(CScrollBackground)->Set("BACKGROUND2", 60);
		Instantiate(CSlowMoveBackground)->Set("BACKGROUND3", 0, 0.2f);

		Instantiate(CObstacle)->SetByWoodBlock(3, 750, 500);
		Instantiate(CObstacle)->SetByWoodBlock(5, 1200, 500);

		Instantiate(CObstacle)->Set("STONE", 135, 500);
		Instantiate(CObstacle)->Set("STONE", 390, 540);

		Instantiate(CObstacle)->Set("STONE", 1700, 540);
		Instantiate(CObstacle)->Set("STONE", 2000, 540);

		Instantiate(CObstacle)->SetByWoodBlock(3, 2300, 500);

		Instantiate(CPlayer)->Setup(100, 270, 0.45f);

		Instantiate(CConch)->Set(650, 270);
		Instantiate(CConch)->Set(1100, 270);

		Instantiate(CFish)->Set(0, 500, 600, 800);
		Instantiate(CFish)->Set(1451, 550, 800, 1450);

		Instantiate(CWaveRenderer)->SetUp(Vector2(0, 600), 800, "WAVE", 10);
		}, [&]() {
			lp_gameobject_list enemy_list = ENTITY.GetObjectList("ENEMY");

			if (enemy_list && enemy_list->size() == 0)
			{
				m_eGameProgress = GAME_PROGRESS::STAGE1_MIDDLE_BOSS;
				m_pStateMachine->ChangeState(m_eGameProgress);
			}
		}, [&]() {});

	// ==============================================
	// SETUP : STAGE1_MIDDLE_BOSS
	// ==============================================
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE1_MIDDLE_BOSS, [&]() {

		CAMERA.SetMapWidth(0, 2500 + screen_width + 100);
		CPlayer::SetReSpawnPos(Vector3(2600, 270, 0));

		Instantiate(CObstacle)->Set("STONE", 2600, 500);
		Instantiate(CObstacle)->Set("STONE", 3530, 500);

		Instantiate(CObstacle)->SetByFence(2680, 230);
		Instantiate(CObstacle)->SetByFence(3450, 230);

		Instantiate(CObstacle)->SetByWoodBlock(5, 3065, 450);
		
		Instantiate(COctopus)->Setup(3065, 100, 100);
		}, [&]() {
			if (CAMERA.GetPosition().x >= 2500 + screen_width / 2.f)
			{
				CAMERA.SetMapWidth(2500, 2500 + screen_width + 100);
			}

			lp_gameobject_list boss_list = ENTITY.GetObjectList("BOSS");

			if (boss_list && boss_list->size() == 0)
			{
				m_eGameProgress = GAME_PROGRESS::STAGE1_SECOND;
				m_pStateMachine->ChangeState(m_eGameProgress);
			}
		}, [&]() {
			
		});

	// ==============================================
	// SETUP : STAGE1_SECOND
	// ==============================================
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE1_SECOND, [&]() {

		CAMERA.SetMapWidth(2500, 3624 + 2400);
		CPlayer::SetReSpawnPos(Vector3(3924, 200, 0));

		Instantiate(CObstacle)->SetByWoodBlock(5, 3989, 450);
		Instantiate(CObstacle)->SetByWoodBlock(5, 4377, 450);
		Instantiate(CObstacle)->SetByWoodBlock(5, 4765, 450);

		Instantiate(CObstacle)->Set("STONE", 5300, 500);
		Instantiate(CObstacle)->Set("STONE", 5591, 500);
		Instantiate(CObstacle)->Set("STONE", 5882, 500);

		Instantiate(CConch)->Set(3989, 270);
		Instantiate(CConch)->Set(4377, 270);
		Instantiate(CConch)->Set(4765, 270);
		Instantiate(CConch)->Set(5300, 270);
		Instantiate(CConch)->Set(5591, 270);
		Instantiate(CConch)->Set(5882, 270);

		Instantiate(CFish)->Set(4200, 500, 4100, 5000);
		Instantiate(CFish)->Set(4600, 550, 4600, 5200);

		ENTITY.FindGameObject("WATER", "WATER")->GC(CWaveRenderer)->ResetWater(Vector2(2500, 600), 690);

		}, [&]() {

			if (CAMERA.GetPosition().x >= 3624 + screen_width / 2.f)
			{
				CAMERA.SetMapWidth(3624, 3624 + 2400);
			}

			lp_gameobject_list enemy_list = ENTITY.GetObjectList("ENEMY");

			if (enemy_list && enemy_list->size() == 0)
			{
				m_eGameProgress = GAME_PROGRESS::STAGE1_FINAL_BOSS;
				m_pStateMachine->ChangeState(m_eGameProgress);
			}
		}, [&]() {});

	// ==============================================
	// SETUP : STAGE1_FINAL_BOSS
	// ==============================================
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE1_FINAL_BOSS, [&]() {
		CAMERA.SetMapWidth(3624, 6124 + screen_width);

		auto obs = Instantiate(CObstacle);
		obs->Set("STONE", 6202, 500);
		obs->GetGameObject()->GC(CSpriteRenderer)->SetRenderLayer(11);
		auto obs2 = Instantiate(CObstacle);
		obs2->Set("STONE", 6490, 600);
		obs2->GetGameObject()->GC(CSpriteRenderer)->SetRenderLayer(11);

		Instantiate(CObstacle)->SetByFence(6537, 300);
		Instantiate(CObstacle)->SetByFence(6537, 100);

		Instantiate(CPianus)->Setup(6950, 400, 200);

		Instantiate(CWaveRenderer)->SetUp(Vector2(6300, 680), 200, "WAVE", 10);

		}, [&]() {

			if (CAMERA.GetPosition().x >= 6124 + screen_width / 2.f)
			{
				CAMERA.SetMapWidth(6124, 6124 + screen_width);
			}

		}, [&]() {
		
		});
}

void CStageAdmin::StateSetup2()
{
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE2_FIRST, [&]() {}, [&]() {}, [&]() {});
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE2_MIDDLE_BOSS, [&]() {}, [&]() {}, [&]() {});
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE2_SECOND, [&]() {}, [&]() {}, [&]() {});
	m_pStateMachine->AddState((INT)GAME_PROGRESS::STAGE2_FINAL_BOSS, [&]() {}, [&]() {}, [&]() {});
}
