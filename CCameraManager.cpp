#include "DXUT.h"
#include "CCameraManager.h"


CCameraManager::CCameraManager()
	: m_vEye(screen_width / 2.f, screen_height / 2.f, 0),
	m_vLookat(0.0f, 0.0f, 0.0f), m_vScale(1.f, 1.f, 1.f), m_vLerpScale(1.f, 1.f, 1.f),
	m_vViewSize(screen_width, screen_height, 0), m_vUp(0.0f, 1.0f, 0.0f), m_pTarget(null)
{
	SetTransform3D();
}


CCameraManager::~CCameraManager()
{
}

void CCameraManager::Update()
{
	if (m_pTarget)
	{
		Vector3 end = *m_pTarget->GetPosition();
		end.y = m_vEye.y;

		lerp(&m_vEye, m_vEye, end, GetDeltaTime() * 3);
	}

	if (m_vEye.x - screen_width / 2 <= m_fMinX)
		m_vEye.x = m_fMinX + screen_width / 2;
	else if (m_vEye.x + screen_width / 2 >= m_fMaxX)
		m_vEye.x = m_fMaxX - screen_width / 2;

	m_matView = D3DXMATRIX(
		m_vScale.x * cosf(m_fAngle), m_vScale.x * sinf(m_fAngle), 0, 0,
		-m_vScale.y * sinf(m_fAngle), m_vScale.y * cosf(m_fAngle), 0, 0,
		0, 0, m_vScale.z, 0,
		-m_vEye.x * m_vScale.y * cosf(m_fAngle) + m_vEye.y * m_vScale.y * sinf(m_fAngle),
		-m_vEye.x * m_vScale.x * sinf(m_fAngle) - m_vEye.y * m_vScale.y * cosf(m_fAngle), 0, 1);
}

void CCameraManager::SetTransform3D()
{
	
}

void CCameraManager::SetTransform2D()
{
	GetD3D9Device()->SetTransform(D3DTS_VIEW, &m_matView);
	GetD3D9Device()->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCameraManager::Initialize3D()
{
	constexpr float fovy = (float)screen_width / (float)screen_height;

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, fovy, 1.0f, 1000.0f);
	GetD3D9Device()->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CCameraManager::Initialize2D()
{
	m_fAngle = 0.0f;

	D3DXMatrixOrthoLH(&m_matProj, m_vViewSize.x, -m_vViewSize.y, 0, 1);
}

void CCameraManager::SetTargetTransform(transform * target)
{
	m_pTarget = target;
}
