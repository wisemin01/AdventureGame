#pragma once
#include "CBaseComponent.h"


class CRigidbody2D :
	public CBaseComponent
{
private:
	bool m_bUseGravity = true;
	bool m_bIsOnStaticObject = false;

	float m_fMess = 1.0f;
	float m_fPowerOfBounce = 0.1f;
	
	float m_fHorizontalAttenuation = 0.98f;
	float m_fVerticalAttenuation = 0.98f;

	Vector3 m_vVelocity = Vector3(0, 0, 0);
	Vector2 m_vTargetFrictionalForce = Vector2(0, 0);

public:
	CRigidbody2D(lp_gameobject owner_obj);
	virtual ~CRigidbody2D();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void AddForce(Vector3 force);
	void AddForce(Vector2 force);
	void AddForce(float x, float y);
	void AddForce(Vector3 force, float power);
	void AddForce(Vector2 force, float power);
	void AddForce(float x, float y, float power);


	void SetMess(float mess) { m_fMess = mess; }
	void SetBouncePower(float bounc) { m_fPowerOfBounce = bounc; }
	void SetVelocity(Vector3 vel) { m_vVelocity = vel; }
	void SetVelocity(float x, float y, float z = 0.0f) { m_vVelocity = Vector3(x, y, z); }

	void SetVelocityX(float x) { m_vVelocity.x = x; }
	void SetVelocityY(float y) { m_vVelocity.y = y; }
	void SetVelocityZ(float z) { m_vVelocity.z = z; }

	void SetUseGravity(bool is_use) { m_bUseGravity = is_use; }

	void Bounce(float power = 1.0f);

	void SetHorizontalAttenuation(float damping)
	{ m_fHorizontalAttenuation = damping; }
	void SetVerticalAttenuation(float damping)
	{ m_fVerticalAttenuation = damping; }

	float GetHorizontalAttenuation() { return m_fHorizontalAttenuation; }
	float GetVerticalAttenuation() { return m_fVerticalAttenuation; }

	float GetBouncePower() { return m_fPowerOfBounce; }

	float GetMess() { return m_fMess; }
	Vector3 GetVelocity() { return m_vVelocity; }

	void InterlockToCollider();

private:
	static constexpr float g_fGravity = 9.8f;

	void CollisionCalcOfStaticObj(CCollider2D* other);
	void CollisionCalcOfNormalObj(CCollider2D* other);

public:
	static constexpr float GetGravity() { return g_fGravity; }
};

