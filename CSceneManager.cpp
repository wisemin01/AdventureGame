#include "DXUT.h"
#include "CSceneManager.h"

#include "CTransform2D.h"
#include "CSpriteRenderer.h"

CSceneManager::CSceneManager()
	: m_pNowScene(null), m_pNextScene(null)
{
	m_bIsLoading = false;
}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::UpdateScenes()
{
	if (m_pNextScene && !m_bIsLoading)
	{
		if (m_pNowScene)
		{
			m_pNowScene->Destroy();
			ENTITY.AllListClear();
			ENTITY.Reset();
			RESOURCE.Clear();
		}

		m_pNowScene = m_pNextScene;
		m_pLoadThread = std::thread([&]() { m_pNowScene->Load(); });
		m_pNextScene = null;
		m_bIsLoading = true;
	}

	if (m_bIsLoading)
	{
		Sleep(1);

		if (m_pNowScene->IsEndLoading())
		{
			console_log("END_LOADING");

			if (m_pLoadThread.joinable())
				m_pLoadThread.join();

			m_bIsLoading = false;
			m_pNowScene->Init();
		}
	}
	else if (m_pNowScene)
	{
		m_pNowScene->Update();

		ENTITY.DispatchMessages();
		COLLISION.CollisionCheck();
		ENTITY.ObjectLoop();
	}
}

void CSceneManager::RenderScenes()
{
	ENTITY.RenderingLoop();
}

void CSceneManager::DestroyScenes()
{
	if (m_pNowScene)
		m_pNowScene->Destroy();

	for (auto iter : m_pSceneContainer)
	{
		SAFE_DELETE(iter.second);
	}

	ENTITY.AllListClear();
	ENTITY.Reset();

	m_pSceneContainer.clear();
}