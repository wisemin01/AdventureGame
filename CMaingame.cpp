#include "DXUT.h"
#include "CMaingame.h"

#include "CTitleScene.h"
#include "CStage1.h"

CMaingame::CMaingame()
{
}


CMaingame::~CMaingame()
{
}

HRESULT CMaingame::Init()
{
	CAMERA.Initialize2D();

	SCENE.AddScene<CTitleScene>();
	SCENE.AddScene<CStage1>();

	SCENE.ChangeScene<CTitleScene>();

	return S_OK;
}

void CMaingame::Update()
{
	CAMERA.Update();
	INPUT.InputUpdate();

	SCENE.UpdateScenes();
}

void CMaingame::Render()
{
	CAMERA.SetTransform2D();
	SCENE.RenderScenes();
}

void CMaingame::Destroy()
{
	SCENE.DestroyScenes();

	CSceneManager::DestInstance();
	CEntityManager::DestInstance();
	CCollisionManager::DestInstance();
	CResourceManager::DestInstance();
	CCameraManager::DestInstance();
	CInputManager::DestInstance();
	CSoundManager::DestInstance();
}

HRESULT CMaingame::Reset()
{
	return S_OK;
}

void CMaingame::Lost()
{
}
