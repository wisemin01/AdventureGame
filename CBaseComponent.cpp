#include "DXUT.h"
#include "CBaseComponent.h"



CBaseComponent::CBaseComponent(lp_gameobject owner_object, const object_id& obj_id)
	: m_pGameObject(owner_object)
{
	if (obj_id.size() != 0)
		GO.SetObjectID(obj_id);
}

CBaseComponent::~CBaseComponent()
{
}

void CBaseComponent::HandleMessage(lp_message msg)
{
	return;
}
