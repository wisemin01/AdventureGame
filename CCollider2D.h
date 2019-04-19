#pragma once
#include "CBaseCollider.h"


class CCollider2D :
	public CBaseCollider
{
private:

	RECT m_rcRange;
	std::list<std::function<void(CCollider2D*)>> m_pOnCollisionList;

	Vector2 m_vFrictionalForce = Vector2(0.1f, 0.1f);

	bool m_bIsStatic = false;
	bool m_bIsPassAble = false;
	bool m_bIsTrigger = false;

	float m_fPushPower = 100.0f;


public:
	CCollider2D(lp_gameobject owner_obj);
	virtual ~CCollider2D();

	virtual void Init() override {}
	virtual void Update() override {}
	virtual void Destroy() override {}

	void SetRange(float range);
	void SetRange(float width, float height);
	void SetRange(float left, float top, float right, float bottom);

	void SetRangeAsImage();

	float GetWidth() { return m_rcRange.right - m_rcRange.left; }
	float GetHeight() { return m_rcRange.bottom - m_rcRange.top; }

	float GetPushPower() { return m_fPushPower; }
	void  SetPushPower(float pwr) { m_fPushPower = pwr; }

	bool GetPassAble() { return m_bIsPassAble; }
	void SetPassAble(bool pass_able) { m_bIsPassAble = pass_able; }

	RECT GetWorldRange() const;

	void Register();
	void UnRegister();

	void AddOnCollisionFunc(std::function<void(CCollider2D*)> pFunc);

	void FuncClear();

	void OnCollision(CCollider2D* other);

	Vector2 GetFrictionalForce() { return m_vFrictionalForce; }
	void SetFrictionalForce(float x, float y) { m_vFrictionalForce.x = x; m_vFrictionalForce.y = y; }

	bool GetIsStaticCollider() { return m_bIsStatic; }
	void SetIsStaticCollider(bool is_static) { m_bIsStatic = is_static; }

	void Setup(float width, float height);
	void Setup(float left, float top, float right, float bottom);
	void Setup();

	void SetIsTrigger(bool value) { m_bIsTrigger = value; }
	bool GetIsTrigger() { return m_bIsTrigger; }
};

