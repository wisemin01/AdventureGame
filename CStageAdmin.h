#pragma once
#include "CBaseComponent.h"

enum class GAME_PROGRESS
{
	STAGE1_FIRST,
	STAGE1_MIDDLE_BOSS,
	STAGE1_SECOND,
	STAGE1_FINAL_BOSS,
	STAGE2_FIRST,
	STAGE2_MIDDLE_BOSS,
	STAGE2_SECOND,
	STAGE2_FINAL_BOSS
};

class CStateMachine;
class CStageAdmin :
	public CBaseComponent
{
protected:
	GAME_PROGRESS m_eGameProgress = GAME_PROGRESS::STAGE1_FIRST;
	CStateMachine * m_pStateMachine = null;
public:
	CStageAdmin(lp_gameobject owner_obj);
	virtual ~CStageAdmin();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void StartStage1();
	void StartStage2();

	void StateSetup1();
	void StateSetup2();
};

