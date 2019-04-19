#pragma once
#include "CBaseComponent.h"
class CBaseTransform :
	public CBaseComponent
{
protected:

	Vector3 m_vPosition;
	Vector3 m_vRotation;
	Vector3 m_vScale;

	Matrix m_matWorld;

public:
	CBaseTransform(lp_gameobject owner_object);
	virtual ~CBaseTransform();

	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	virtual void SetTransform();

	Vector3* GetPosition() { return &m_vPosition; }
	Vector3* GetRotation() { return &m_vRotation; }
	Vector3* GetScale	() { return &m_vScale;	  }

	void SetPosition(const Vector3& position) { m_vPosition = position; }
	void SetRotation(const Vector3& rotation) { m_vRotation = rotation; }
	void SetScale(const Vector3& scale) { m_vScale = scale; }

	void SetPosition(float x, float y, float z = 0) { m_vPosition = Vector3(x, y, z); }
	void SetRotation(float x, float y, float z) { m_vRotation = Vector3(x, y, z); }
	void SetScale(float x, float y, float z = 0) { m_vScale = Vector3(x, y, z); }

	void SetRotation(float rot) { m_vRotation.z = rot; }

public:
	static void SetTransformDefault(Vector3 s, Vector3 t, Vector3 r);

	static Vector3 GetDirection(CBaseTransform* tr1, CBaseTransform* tr2);
};

