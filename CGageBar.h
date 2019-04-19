#pragma once
#include "CSpriteRenderer.h"

class CLifeMachine;
class CGageBar :
	public CSpriteRenderer
{
protected:
	CLifeMachine * m_pTargetLifeMachine = null;

	Vector3 * m_vTargetPos;

	float m_fLerpWidth;
	float m_fWidth;
	float m_fHeight;
public:
	CGageBar(lp_gameobject owner_obj);
	virtual ~CGageBar();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void Setup(CGameObject* target_obj, Vector3* target_pos, Vector3 offset, const resource_id& rc_id);
};

