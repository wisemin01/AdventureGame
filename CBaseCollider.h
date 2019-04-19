#pragma once
#include "CBaseComponent.h"


class CBaseCollider :
	public CBaseComponent
{
private:

public:
	CBaseCollider(lp_gameobject owner_obj);
	virtual ~CBaseCollider();

	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Destroy() PURE;
};

