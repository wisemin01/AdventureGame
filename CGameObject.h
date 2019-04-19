#pragma once
#include "CObject.h"

class CGameObject :
	public CObject
{
	friend class CEntityManager;

protected:
	is_live		m_bIsLive = true;
	is_enable	m_bIsEnable = true;

protected:
	renderer * m_pRenderer = nullptr;
	transform * m_pTransform = nullptr;

	component_map m_pComponentContainer;

public:
	CGameObject();
	virtual ~CGameObject();

	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	virtual void HandleMessage(lp_message msg);

	void DefaultMessageHandler(lp_message msg);

	void ComponentUpdate();
	void ComponentDestroy();

	void Kill() { m_bIsLive = false; }
	void Kill(lp_gameobject other) { other->m_bIsLive = false; }

	bool IsLive() { return m_bIsLive; }

	template <class T>
	T* AddComponent()
	{
		component_id com_id = typeid(T).name();

		auto find = m_pComponentContainer.find(com_id);

		if (find == m_pComponentContainer.end()) {

			T* com = new T(this);

			com->Init();

			m_pComponentContainer.insert(std::make_pair(com_id, com));
			return com;
		}
		else
			return nullptr;
	}

	template <class T>
	T* GetComponent()
	{
		component_id com_id = typeid(T).name();

		auto find = m_pComponentContainer.find(com_id);

		if (find == m_pComponentContainer.end())
			return nullptr;
		else
			return reinterpret_cast<T*>(find->second);
	}

	template <class T>
	void DeleteComponent()
	{
		component_id com_id = typeid(T).name();

		auto find = m_pComponentContainer.find(com_id);

		if (find == m_pComponentContainer.end())
			return;
		else
		{
			find->second->Destroy();
			safe_delete(find->second);
			m_pComponentContainer.erase(find);
		}
	}

	renderer* GetRenderer() { return m_pRenderer; }
	transform* GetTransform() { return m_pTransform; }

	void SetRenderer(renderer* renderer_ptr) { m_pRenderer = renderer_ptr; }
	void SetTransform(transform* transform_ptr) { m_pTransform = transform_ptr; }
};

