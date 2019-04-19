#pragma once
#include "CBaseComponent.h"

// »¡°­ - 3¹æÇâ
// ÃÊ·Ï - ÇÙ
// ¹ö¼¸ - À¯µµ

class CRigidbody2D;
class CCollider2D;

class CBullet :
	public CBaseComponent
{
protected:
	Vector2 m_vDir;

	float m_fSpeed = 0.0f;
	float m_fDamage = 0.0f;
	float m_fDeathTime = 0.0f;
	float m_fSoundTime = 0.0f;

	int m_iAttackCount;

	CCollider2D* m_pCollider = null;
	CRigidbody2D* m_pRigidbody = null;
public:
	CBullet(lp_gameobject owner_obj);
	virtual ~CBullet();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(Vector3 position, Vector2 diretion, const resource_id& id, float speed, float damage, int attack_count = 1);

	virtual void OnCollision(CCollider2D* other);
};

