#include "DXUT.h"
#include "CGameObject.h"

#include "CBaseComponent.h"
#include "CBaseRenderer.h"


CGameObject::CGameObject()
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::Init()
{
}

void CGameObject::Update()
{
}

void CGameObject::Destroy()
{
}

void CGameObject::HandleMessage(lp_message msg)
{
	for (auto iter : m_pComponentContainer)
	{
		iter.second->HandleMessage(msg);

		if (msg == null)
			return;
	}

	return DefaultMessageHandler(msg);
}

void CGameObject::DefaultMessageHandler(lp_message msg)
{
	if (!msg)
		return;

	switch (msg->GetType())
	{
	case eMsgDestroy:
		Kill();
		break;
	}
}

void CGameObject::ComponentUpdate()
{
	for (auto iter : m_pComponentContainer)
		(iter.second)->Update();
}

void CGameObject::ComponentDestroy()
{
	for (auto iter : m_pComponentContainer)
	{
		iter.second->Destroy();
		safe_delete(iter.second);
	}
	m_pComponentContainer.clear();
}
