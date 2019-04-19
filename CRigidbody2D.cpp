#include "DXUT.h"
#include "CRigidbody2D.h"
#include "CCollider2D.h"

#define DAMPING (0.99f)

#define NORMAL_ATTENUATION (0.98f)

CRigidbody2D::CRigidbody2D(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj)
{

}


CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::Init()
{
	InterlockToCollider();
}

void CRigidbody2D::Update()
{
	Vector3* pos = GO.GetTransform()->GetPosition();

	if (m_bUseGravity)
		m_vVelocity.y += g_fGravity * m_fMess;

	Vector3 endPos = *pos + m_vVelocity * GetDeltaTime();

	if (!m_bIsOnStaticObject)
	{
		m_vTargetFrictionalForce.x = 0;
		m_vTargetFrictionalForce.y = 0;
	}

	*pos = endPos;

	m_vVelocity.x *= (m_fHorizontalAttenuation - m_vTargetFrictionalForce.x);
	m_vVelocity.y *= (m_fVerticalAttenuation - m_vTargetFrictionalForce.y);

	m_bIsOnStaticObject = false;
}

void CRigidbody2D::Destroy()
{
}

void CRigidbody2D::AddForce(Vector3 force)
{
	m_vVelocity += force;
}

void CRigidbody2D::AddForce(Vector2 force)
{
	m_vVelocity += Vector3(force.x, force.y, 0);
}

void CRigidbody2D::AddForce(float x, float y)
{
	m_vVelocity.x += x;
	m_vVelocity.y += y;
}

void CRigidbody2D::AddForce(Vector3 force, float power)
{
	m_vVelocity += force * power;
}

void CRigidbody2D::AddForce(Vector2 force, float power)
{
	m_vVelocity += Vector3(force.x * power, force.y * power, 0);
}

void CRigidbody2D::AddForce(float x, float y, float power)
{
	m_vVelocity.x += x * power;
	m_vVelocity.y += y * power;
}

void CRigidbody2D::Bounce(float power)
{
	m_vVelocity.y = -m_fPowerOfBounce * m_vVelocity.y * power;
}

void CRigidbody2D::InterlockToCollider()
{
	CCollider2D* collider2D = GO.GC(CCollider2D);

	if (collider2D)
	{
		collider2D->AddOnCollisionFunc([&](CCollider2D* other) {

			if (other->GetIsTrigger())
				return;

			if (other->GetIsStaticCollider())
				CollisionCalcOfStaticObj(other);
			else
				CollisionCalcOfNormalObj(other);
			});
	}
}

void CRigidbody2D::CollisionCalcOfStaticObj(CCollider2D * other)
{
	auto backupFrictionalForce = m_vTargetFrictionalForce;
	m_vTargetFrictionalForce = other->GetFrictionalForce();

	CCollider2D* this_collider = GO.GC(CCollider2D);

	m_bIsOnStaticObject = true;

	RECT out_rc;

	RECT this_rc = this_collider->GetWorldRange();
	RECT obstacle_rc = other->GetWorldRange();

	Vector3 this_pos = *GO.GetTransform()->GetPosition();
	Vector3 obstacle_pos = *other->m_pGameObject->GetTransform()->GetPosition();

	float power = m_fPowerOfBounce * other->GetPushPower();

	if (IntersectRect(&out_rc, &this_rc,
		&obstacle_rc))
	{
		int width = out_rc.right - out_rc.left;
		int height = out_rc.bottom - out_rc.top;

		if (width > height)
		{
			// y 面倒

			if (this_pos.y >= obstacle_pos.y)
			{
				if (!other->GetPassAble())
					m_vVelocity.y += height * power * GetDeltaTime();
				else
					m_vTargetFrictionalForce = backupFrictionalForce;
			}
			else if (this_pos.y < obstacle_pos.y)
			{
				m_vVelocity.y -= height * power * GetDeltaTime();
			}

		}
		else
		{
			// x 面倒

			if (this_pos.x >= obstacle_pos.x)
			{
				m_vVelocity.x += width * power * GetDeltaTime() * 0.5;
			}
			else if (this_pos.x < obstacle_pos.x)
			{
				m_vVelocity.x -= width * power * GetDeltaTime() * 0.5;
			}

		}

	}
}

void CRigidbody2D::CollisionCalcOfNormalObj(CCollider2D * other)
{
	m_bIsOnStaticObject = false;

	CCollider2D* this_collider = GO.GC(CCollider2D);

	RECT out_rc;

	RECT this_rc = this_collider->GetWorldRange();
	RECT other_rc = other->GetWorldRange();

	Vector3 this_pos = *GO.GetTransform()->GetPosition();
	Vector3 other_pos = *other->m_pGameObject->GetTransform()->GetPosition();

	CGameObject* other_obj = other->GetGameObject();
	CRigidbody2D* other_rigidbody = other_obj->GC(CRigidbody2D);

	if (!other_rigidbody)
		return;

	float power_to_this = m_fPowerOfBounce
		* other->GetPushPower()
		* other_rigidbody->GetMess();

	if (this_collider->GetIsStaticCollider())
		power_to_this = 0.0f;

	float power_to_other = -other_rigidbody->GetBouncePower()
		* this_collider->GetPushPower()
		* GetMess();

	if (IntersectRect(&out_rc, &this_rc,
		&other_rc))
	{
		int width = out_rc.right - out_rc.left;
		int height = out_rc.bottom - out_rc.top;

		if (width > height)
		{
			// y 面倒

			if (this_pos.y >= other_pos.y)
			{
				AddForce(0, height * power_to_this * GetDeltaTime());
				other_rigidbody->AddForce(0, height * power_to_other * GetDeltaTime());
			}
			else if (this_pos.y < other_pos.y)
			{
				AddForce(0, -height * power_to_this * GetDeltaTime());
				other_rigidbody->AddForce(0, -height * power_to_other * GetDeltaTime());
			}

		}
		else
		{
			// x 面倒

			if (this_pos.x >= other_pos.x)
			{
				AddForce(width * power_to_this * GetDeltaTime(), 0);
				other_rigidbody->AddForce(width * power_to_other * GetDeltaTime(), 0);
			}
			else if (this_pos.x < other_pos.x)
			{
				AddForce(-width * power_to_this * GetDeltaTime(), 0);
				other_rigidbody->AddForce(-width * power_to_other * GetDeltaTime(), 0);
			}

		}
	}
}
