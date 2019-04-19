#pragma once
#include "CBullet.h"

class CStateMachine;

enum class TARGET_BULLET_STATE
{
	DETECT,
	FOLLOW,
	DEATH
};

class CTargetBullet :
	public CBullet
{
protected:
	CStateMachine * m_pStateMachine = null;

	CGameObject* m_pTarget = null;

	float m_fCalcTime = 0;
	float m_fTimeRot = 0;
public:
	CTargetBullet(lp_gameobject owner_obj);
	virtual ~CTargetBullet();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(Vector3 position, const resource_id& id, float speed, float damage);
};

