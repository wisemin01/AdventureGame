#pragma once
#include "CScene.h"
class CTitleScene :
	public CScene
{
public:
	CTitleScene();
	~CTitleScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void HandleMessage(lp_message msg) override;

	virtual void Load() override;
};

