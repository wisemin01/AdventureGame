#include "DXUT.h"
#include "CBaseTransform.h"



CBaseTransform::CBaseTransform(lp_gameobject owner_object)
	: CBaseComponent(owner_object), m_vPosition(Vector3(0, 0, 0)),
	m_vRotation(Vector3(0, 0, 0)), m_vScale(Vector3(1.0, 1.0, 1.0))
{
	SetTransform();
	m_pGameObject->SetTransform(this);
}

CBaseTransform::~CBaseTransform()
{
	if (m_pGameObject->GetTransform() == this)
		m_pGameObject->SetTransform(null);
}

void CBaseTransform::Init()
{
}

void CBaseTransform::Update()
{
}

void CBaseTransform::Destroy()
{
}

void CBaseTransform::SetTransform()
{
	D3DXMATRIX S, Rx, Ry, Rz, R, T;

	D3DXMatrixScaling(&S, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXMatrixRotationX(&Rx, m_vRotation.x);
	D3DXMatrixRotationY(&Ry, m_vRotation.y);
	D3DXMatrixRotationZ(&Rz, m_vRotation.z);

	R = Rx * Ry * Rz;

	D3DXMatrixTranslation(&T, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = S * R * T;

	GetD3D9Device()->SetTransform(D3DTS_WORLD, &m_matWorld);
}

void CBaseTransform::SetTransformDefault(Vector3 s, Vector3 t, Vector3 r)
{
	D3DXMATRIX S, R, T;

	D3DXMatrixScaling(&S, s.x, s.y, s.z);

	D3DXMatrixRotationZ(&R, D3DXToRadian(r.z));

	D3DXMatrixTranslation(&T, t.x, t.y, t.z);

	D3DXMATRIX matWorld = S * R * T;

	RESOURCE.GetSprite()->SetTransform(&matWorld);
}

Vector3 CBaseTransform::GetDirection(CBaseTransform * tr1, CBaseTransform * tr2)
{
	Vector3 dir = (*tr1->GetPosition()) - (*tr2->GetPosition());

	D3DXVec3Normalize(&dir, &dir);
	return dir;
}
