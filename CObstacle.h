#pragma once
#include "CBaseComponent.h"

class CCollider2D;
class CObstacle :
	public CBaseComponent
{
protected:

	float m_fWidth = 0;
	float m_fHeight = 0;

public:
	CObstacle(lp_gameobject owner_obj);
	virtual ~CObstacle();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Set(const resource_id& id, float x, float y, Vector2 attenuation = Vector2(0.03f, 0.3f));

	void SetByWoodBlock(int size, float x, float y, Vector2 attenuation = Vector2(0.03f, 0.3f));
	void SetByFence(float x, float y, Vector2 attenuation = Vector2(0.03f, 0.3f));

	void OnCollision(CCollider2D* other);
};

