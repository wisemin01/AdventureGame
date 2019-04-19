#pragma once
#include "CSingleton.h"

class CCameraManager :
	public CSingleton<CCameraManager>
{
private:

	Vector3 m_vEye;
	Vector3 m_vLookat;
	Vector3 m_vUp;
	Vector3 m_vOffset;
	Vector3 m_vScale;
	Vector3 m_vLerpScale;
	Vector3 m_vViewSize;

	transform* m_pTarget = null;

	Matrix m_matView;
	Matrix m_matProj;

	float m_fAngle;

	float m_fMinX;
	float m_fMaxX;

public:
	CCameraManager();
	virtual ~CCameraManager();

	void Update();

	void SetTransform3D();
	void SetTransform2D();

	void Initialize3D();
	void Initialize2D();

	void SetMapWidth(float min, float max) { m_fMinX = min; m_fMaxX = max; }

	// Get Map Width (min_x, max_x)
	std::tuple<float, float> GetMapWidth() { return std::make_tuple(m_fMinX, m_fMaxX); }

	void SetTargetTransform(transform* target);
	transform* GetTargetTransform() { return m_pTarget; }

	Vector3 GetPosition() { return m_vEye; }
	void SetPosition(float x, float y, float z = 0.0f) { m_vEye = Vector3(x, y, z); }
	void SetPosition(Vector3 pos) { m_vEye = pos; }
};

#define CAMERA (*CCameraManager::Instance())