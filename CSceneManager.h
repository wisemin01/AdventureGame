#pragma once
#include "CSingleton.h"

class CSceneManager :
	public CSingleton<CSceneManager>
{

private:

	std::unordered_map<scene_id, lp_scene> m_pSceneContainer;
	lp_scene m_pNowScene;
	lp_scene m_pNextScene;

	std::thread m_pLoadThread;
	bool m_bIsLoading;

public:

	template <class T>
	T* AddScene()
	{
		static_assert(std::is_base_of<scene, T>::value,
			" �� ������ Ŭ������ �� �����̳ʿ� �����Ҽ� �����ϴ�.");

		T* _ptr = new T;

		m_pSceneContainer.insert(std::make_pair(typeid(T).name(), _ptr));

		return _ptr;
	}

	template <class T>
	T* ChangeScene()
	{
		auto find = m_pSceneContainer.find(typeid(T).name());

		m_pNextScene = find->second;

		return reinterpret_cast<T*>(m_pNextScene);
	}

	void UpdateScenes();
	void RenderScenes();

	void DestroyScenes();

	lp_scene GetNowScene() { return m_pNowScene; }
public:
	CSceneManager();
	virtual ~CSceneManager();
};

#define SCENE (*CSceneManager::Instance())