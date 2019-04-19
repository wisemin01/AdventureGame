#pragma once


class CBaseComponent
{
public:
	lp_gameobject m_pGameObject = null;
public:
	CBaseComponent(lp_gameobject owner_object, const object_id& obj_id = "");
	virtual ~CBaseComponent();

	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Destroy() PURE;

	virtual void HandleMessage(lp_message msg);

	lp_gameobject GetGameObject()
	{
		return m_pGameObject;
	}
};