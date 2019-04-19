#pragma once
#include "CScene.h"
class CStage1 :
	public CScene
{
public:
	CStage1();
	~CStage1();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void HandleMessage(lp_message msg) override;

	virtual void Load() override;
};

