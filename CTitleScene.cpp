#include "DXUT.h"
#include "CTitleScene.h"

#include "CTitleObject.h"

CTitleScene::CTitleScene()
{
}


CTitleScene::~CTitleScene()
{
}

void CTitleScene::Init()
{
	CAMERA.Initialize2D();
	CAMERA.SetPosition(screen_width / 2.f, screen_height / 2.f);
	CAMERA.SetMapWidth(0, screen_width);

	Instantiate(CTitleObject);

	SOUND.Play("TITLE", TRUE);
}

void CTitleScene::Update()
{
}

void CTitleScene::Destroy()
{
	SOUND.Stop("TITLE");
}

void CTitleScene::HandleMessage(lp_message msg)
{
	return;
}

void CTitleScene::Load()
{
	RESOURCE.AddTexture("STAR", "./Resource/Intro/%d.png", 5);
	RESOURCE.AddTexture("BACKGROUND", "./Resource/Intro/bg.png");

	RESOURCE.AddTexture("HUMAN", "./Resource/Intro/h%d.png", 2);

	RESOURCE.AddTexture("TITLE", "./Resource/Intro/title.png");

	SOUND.AddSound("TITLE", "./Resource/Sound/title.wav");

	EndLoading();
}
