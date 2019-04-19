#pragma once
#include "CSingleton.h"

class CEntityManager :
	public CSingleton<CEntityManager>
{
private:

	std::set<message> m_messageList;

public:

	void QuickSend(message_type type, lp_object sender,
		lp_object listener, lp_void extra_info = null, bool is_auto_delete = false);
	void QuickSend(message_type type, lp_object target_obj,
		lp_void extra_info = null, bool is_auto_delete = false);
	void Send(message_type type, lp_object sender,
		lp_object listener, unsigned long send_delay, lp_void extra_info = null,
		bool is_auto_delete = false);
	void Send(message_type type, lp_object target_obj,
		unsigned long send_delay, lp_void extra_info = null,
		bool is_auto_delete = false);

	void SendMessageToAll(message_type type, lp_object sender);

	void DispatchMessages();

	void Reset();

private:

	std::unordered_map<object_id, lp_gameobject_list> m_pObjectContainer;
	render_list m_pRenderContainer;

public:

	void ObjectLoop();
	void RenderingLoop();
	void ObjectClear();
	void AllListClear();
	void ListClear(object_id obj_id);

	template <class T>
	T* AddObject(T* object, object_tag obj_tag = "")
	{
		object->Init();
		object->SetObjectTag(obj_tag);

		constexpr object_id obj_id = object->GetObjectID();
		auto find = m_pObjectContainer.find(obj_id);

		if (find == m_pObjectContainer.end())
			m_pObjectContainer.insert(make_pair(obj_id, new gameobject_list));

		m_pObjectContainer[obj_id]->emplace_back(object);

		return object;
	}

	lp_gameobject CreateDefault(object_id obj_id, object_tag obj_tag = "")
	{
		lp_gameobject ret = new gameobject;
		ret->SetObjectID(obj_id);
		ret->SetObjectTag(obj_tag);
		ret->Init();

		auto find = m_pObjectContainer.find(obj_id);

		if (find == m_pObjectContainer.end())
			m_pObjectContainer.insert(make_pair(obj_id, new gameobject_list));

		m_pObjectContainer[obj_id]->emplace_back(ret);

		return ret;
	}

	template <class T>
	T* CreateDefault()
	{
		lp_gameobject ret = new gameobject;
	
		T* ret_com = ret->AddComponent<T>();
		ret->Init();

		auto find = m_pObjectContainer.find(ret->GetObjectID());

		if (find == m_pObjectContainer.end())
			m_pObjectContainer.insert(make_pair(ret->GetObjectID(), new gameobject_list));

		m_pObjectContainer[ret->GetObjectID()]->emplace_back(ret);

		return ret_com;
	}

	lp_gameobject_list GetObjectList(object_id obj_id)
	{
		return m_pObjectContainer[obj_id];
	}

	lp_gameobject FindGameObject(object_tag obj_tag);
	lp_gameobject FindGameObject(object_id obj_id, object_tag obj_tag);

	void DebugList();
public:
	CEntityManager();
	virtual ~CEntityManager();
};

#define ENTITY (*CEntityManager::Instance())

#define Instantiate(type) ENTITY.CreateDefault<type>()

#define CreateEmpty(list_id, obj_tag) ENTITY.CreateDefault(list_id, obj_tag)