#include "DXUT.h"
#include "CEntityManager.h"


CEntityManager::CEntityManager()
{
}


CEntityManager::~CEntityManager()
{
}

void CEntityManager::QuickSend(message_type type, lp_object sender, lp_object listener, lp_void extra_info, bool is_auto_delete)
{
	message send_object(type, sender, listener, extra_info, is_auto_delete);

	send_object.SetTime(0);

	if (listener)
		listener->HandleMessage(&send_object);
}

void CEntityManager::QuickSend(message_type type, lp_object target_obj, lp_void extra_info, bool is_auto_delete)
{
	message send_object(type, target_obj, target_obj, extra_info, is_auto_delete);

	send_object.SetTime(0);

	if (target_obj)
		target_obj->HandleMessage(&send_object);
}

void CEntityManager::Send(message_type type, lp_object sender, lp_object listener, unsigned long send_delay, lp_void extra_info, bool is_auto_delete)
{
	message send_object(type, sender, listener, extra_info, is_auto_delete);

	send_object.SetTime(now_time + send_delay);

	m_messageList.insert(send_object);
}

void CEntityManager::Send(message_type type, lp_object target_obj, unsigned long send_delay, lp_void extra_info, bool is_auto_delete)
{
	message send_object(type, target_obj, target_obj, extra_info, is_auto_delete);

	send_object.SetTime(now_time + send_delay);

	m_messageList.insert(send_object);
}

void CEntityManager::SendMessageToAll(message_type type, lp_object sender)
{
	for(auto iter : m_pObjectContainer)
	{
		for (auto iter2 : *iter.second)
		{
			message send_object(type, sender, iter2, null, false);

			send_object.SetTime(0);

			if (iter2)
				iter2->HandleMessage(&send_object);
		}
	}
}

void CEntityManager::DispatchMessages()
{
	unsigned long cur_time = now_time;

	for (auto &iter : m_messageList)
	{
		if (iter.m_ulDispatchTime <= cur_time)
		{
			message msg = *m_messageList.begin();
			
			if (iter.m_pListener)
			{
				iter.m_pListener->HandleMessage(&msg);

				m_messageList.erase(m_messageList.begin());
			}

			return;
		}
	}
}

void CEntityManager::Reset()
{
	m_messageList.clear();
}


void CEntityManager::ObjectLoop()
{
	m_pRenderContainer.clear();

	for (auto iter : m_pObjectContainer)
	{
		lp_gameobject_list list = iter.second;

		for (auto obj_iter = list->begin(); obj_iter != list->end();)
		{
			gameobject* obj = *obj_iter;
			// 살아있는 오브젝트
			if (obj->m_bIsLive)
			{
				// 켜진 오브젝트 -> 업데이트 + 렌더링
				if (obj->m_bIsEnable) {
					obj->Update();
					obj->ComponentUpdate();

					if (obj->m_pRenderer)
						m_pRenderContainer.emplace_back(obj->m_pRenderer);
				}

				obj_iter++;
			}
			// 파괴된 오브젝트 -> 리스트에서 제거
			else
			{
				obj->Destroy();
				obj->ComponentDestroy();
				safe_delete(obj);
				obj_iter = list->erase(obj_iter);
			}
		}
	}

	// 렌더 리스트 정렬

	m_pRenderContainer.sort(CBaseRenderer::CompareAsLayer);
	m_pRenderContainer.sort(CBaseRenderer::CompareAsY);
}

void CEntityManager::RenderingLoop()
{
	for (auto iter : m_pRenderContainer)
	{
		iter->Render();
	}

	RESOURCE.SpriteEnd();
}

void CEntityManager::ObjectClear()
{
	for (auto iter : m_pObjectContainer)
	{
		for (auto obj_iter : *iter.second)
		{
			obj_iter->Destroy();
			obj_iter->ComponentDestroy();
			safe_delete(obj_iter);
		}
		iter.second->clear();
	}
	m_pRenderContainer.clear();
}

void CEntityManager::AllListClear()
{
	for (auto iter : m_pObjectContainer)
	{
		for (auto obj_iter : *iter.second)
		{
			obj_iter->Destroy();
			obj_iter->ComponentDestroy();
			safe_delete(obj_iter);
		}
		iter.second->clear();
		safe_delete(iter.second);
	}
	m_pObjectContainer.clear();
	m_pRenderContainer.clear();
}

void CEntityManager::ListClear(object_id obj_id)
{
	lp_gameobject_list list = m_pObjectContainer[obj_id];

	for (auto iter : *list)
	{
		iter->Destroy();
		iter->ComponentDestroy();
		safe_delete(iter);
	}
	list->clear();
}

lp_gameobject CEntityManager::FindGameObject(object_tag obj_tag)
{
	for (auto iter : m_pObjectContainer)
	{
		for (auto obj_iter : *iter.second)
		{
			if (obj_iter->GetObjectTag() == obj_tag)
				return obj_iter;
		}
	}

	console_log(obj_tag << " 의 태그를 가진 오브젝트를 찾지 못했습니다.");

	return null;
}

lp_gameobject CEntityManager::FindGameObject(object_id obj_id, object_tag obj_tag)
{
	auto find = m_pObjectContainer.find(obj_id);

	if (find == m_pObjectContainer.end())
		console_log(obj_id << " 의 아이디를 가진 리스트가 없습니다.");

	for (auto iter : *find->second)
	{
		if (iter->GetObjectTag() == obj_tag)
			return iter;
	}

	console_log(obj_tag << " 의 태그를 가진 오브젝트를 찾지 못했습니다.");

	return null;
}

void CEntityManager::DebugList()
{
	std::cout << std::endl;
	std::cout << "============ DEBUG GAMEOBJECT LIST ============" << std::endl;

	int list_count = 0;
	int object_count = 0;

	for (auto iter : m_pObjectContainer)
	{
		std::cout << std::endl;
		console_log("   [" + std::to_string(++list_count) + "] LIST ID : " << iter.first);

		int count = 0;

		for (auto obj_iter : *iter.second)
		{
			console_log("       →" + std::to_string(++count) +
				". OBJECT : TAG = [" << obj_iter->GetObjectTag() <<
				"]" << " STATE : " << (obj_iter->m_bIsEnable ? "ENABLE" : "DISABLE"));
		}

		object_count += count;
	}
	std::cout << std::endl;

	std::cout << "* LIST COUNT : " << list_count << std::endl;
	std::cout << "* OBJECT COUNT : " << object_count << std::endl;

	std::cout << std::endl;
	std::cout << "===============================================" << std::endl;
	std::cout << std::endl;
}
