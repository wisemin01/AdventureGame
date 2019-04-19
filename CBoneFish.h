#pragma once
#include "CTargetBullet.h"

class CSpriteRenderer;
class CBoneFish :
	public CTargetBullet
{
protected:
	CSpriteRenderer * m_pSpriteRenderer;
public:
	CBoneFish(lp_gameobject owner_obj);
	virtual ~CBoneFish();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(Vector3 position, const resource_id & id, float speed, float damage);

	void OnCollision(CCollider2D* other);
};

