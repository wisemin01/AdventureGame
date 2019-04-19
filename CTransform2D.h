#pragma once
#include "CBaseTransform.h"
class CTransform2D :
	public CBaseTransform
{
public:
	CTransform2D(lp_gameobject owner_obj);
	virtual ~CTransform2D();

	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	virtual void SetTransform();

};

