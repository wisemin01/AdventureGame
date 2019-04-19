#include "DXUT.h"
#include "CStage1.h"


#include "CStageAdmin.h"

CStage1::CStage1()
{
}


CStage1::~CStage1()
{
}

void CStage1::Init()
{
	Instantiate(CStageAdmin)->StartStage1();
}

void CStage1::Update()
{
	if (INPUT.GetKeyDown(VK_F11))
		ENTITY.DebugList();
}

void CStage1::Destroy()
{
}

void CStage1::HandleMessage(lp_message msg)
{
}

void CStage1::Load()
{
	RESOURCE.AddTexture("PLAYER_3DIR_A", "./Resource/Player/3direction/a/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_3DIR_I", "./Resource/Player/3direction/i/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_3DIR_W", "./Resource/Player/3direction/w/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_3DIR_J", "./Resource/Player/3direction/j/%d.png", 5);

	RESOURCE.AddTexture("PLAYER_DEFAULT_A", "./Resource/Player/default/a/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_DEFAULT_I", "./Resource/Player/default/i/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_DEFAULT_W", "./Resource/Player/default/w/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_DEFAULT_J", "./Resource/Player/default/j/%d.png", 5);

	RESOURCE.AddTexture("PLAYER_NUCLEUS_A", "./Resource/Player/nucleus/a/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_NUCLEUS_I", "./Resource/Player/nucleus/i/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_NUCLEUS_W", "./Resource/Player/nucleus/w/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_NUCLEUS_J", "./Resource/Player/nucleus/j/%d.png", 5);

	RESOURCE.AddTexture("PLAYER_TARGET_A", "./Resource/Player/target/a/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_TARGET_I", "./Resource/Player/target/i/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_TARGET_W", "./Resource/Player/target/w/%d.png", 5);
	RESOURCE.AddTexture("PLAYER_TARGET_J", "./Resource/Player/target/j/%d.png", 5);

	RESOURCE.AddTexture("BACKGROUND1", "./Resource/Background/Stage1/1.png");
	RESOURCE.AddTexture("BACKGROUND2", "./Resource/Background/Stage1/2.png");
	RESOURCE.AddTexture("BACKGROUND3", "./Resource/Background/Stage1/3.png");

	RESOURCE.AddTexture("CONCH_A", "./Resource/Enemy/Conch/a/%d.png", 5);
	RESOURCE.AddTexture("CONCH_D", "./Resource/Enemy/Conch/d/%d.png", 5);
	RESOURCE.AddTexture("CONCH_W", "./Resource/Enemy/Conch/w/%d.png", 5);

	RESOURCE.AddTexture("FISH_A", "./Resource/Enemy/Fish/a/%d.png", 5);
	RESOURCE.AddTexture("FISH_D", "./Resource/Enemy/Fish/d/%d.png", 5);
	RESOURCE.AddTexture("FISH_W", "./Resource/Enemy/Fish/w/%d.png", 5);

	RESOURCE.AddTexture("OCTOPUS_A1", "./Resource/Boss/Octopus/a1/%d.png", 5);
	RESOURCE.AddTexture("OCTOPUS_A2", "./Resource/Boss/Octopus/a2/%d.png", 5);
	RESOURCE.AddTexture("OCTOPUS_D", "./Resource/Boss/Octopus/d/%d.png", 5);

	RESOURCE.AddTexture("PIANUS_I", "./Resource/Boss/Pianus/i/%d.png", 5);
	RESOURCE.AddTexture("PIANUS_A", "./Resource/Boss/Pianus/a/%d.png", 5);
	RESOURCE.AddTexture("PIANUS_D", "./Resource/Boss/Pianus/d/%d.png", 5);

	RESOURCE.AddTexture("WAVE", "./Resource/Background/Stage1/wave.png");

	RESOURCE.AddTexture("WOOD1", "./Resource/Background/Stage1/wood1.png");
	RESOURCE.AddTexture("WOOD3", "./Resource/Background/Stage1/wood3.png");
	RESOURCE.AddTexture("WOOD5", "./Resource/Background/Stage1/wood5.png");

	RESOURCE.AddTexture("STONE", "./Resource/Background/Stage1/stone.png");
	RESOURCE.AddTexture("BUSH", "./Resource/Background/Stage1/bush.png");
	RESOURCE.AddTexture("FENCE", "./Resource/Background/Stage1/fence.png");

	RESOURCE.AddTexture("BULLET", "./Resource/Bullet/bullet.png");
	RESOURCE.AddTexture("FIRE_BULLET", "./Resource/Bullet/fire_bullet.png");
	RESOURCE.AddTexture("DARK_BULLET", "./Resource/Bullet/dark_bullet.png");
	RESOURCE.AddTexture("COCONUT1", "./Resource/Bullet/coconut1.png");
	RESOURCE.AddTexture("COCONUT2", "./Resource/Bullet/coconut2.png");
	RESOURCE.AddTexture("TENTACLE", "./Resource/Bullet/tentacle.png");
	RESOURCE.AddTexture("LASER", "./Resource/Bullet/laser/%d.png", 5);
	RESOURCE.AddTexture("BONE_FISH", "./Resource/Bullet/bone_fish.png", 5);

	RESOURCE.AddTexture("GAGE_BAR", "./Resource/UI/bar%d.png", 3);
	RESOURCE.AddTexture("BIG_GAGE_BAR", "./Resource/UI/bbar%d.png", 3);

	SOUND.AddSound("STAGE1_BG", "./Resource/Sound/Stage1.wav");
	SOUND.AddSound("JUMP", "./Resource/Sound/jump.wav");
	SOUND.AddSound("HIT", "./Resource/Sound/hit.wav");
	SOUND.AddSound("FIRE_BALL", "./Resource/Sound/fireball.wav");
	SOUND.AddSound("WATER", "./Resource/Sound/water.wav");
	SOUND.AddSound("BOSS", "./Resource/Sound/boss.wav");
	SOUND.AddSound("CLEAR", "./Resource/Sound/clear.wav");

	EndLoading();
}
