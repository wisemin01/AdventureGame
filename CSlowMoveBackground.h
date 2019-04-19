#pragma once
#include "CBaseComponent.h"

class CSlowMoveBackground :
	public CBaseComponent
{
private:
	float m_fSlowRed;
	float m_fImageWidth;
public:
	CSlowMoveBackground(lp_gameobject owner_obj);
	virtual ~CSlowMoveBackground();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Set(const resource_id& id, float startX, float slow_red);

	virtual void HandleMessage(lp_message msg) override;
};

