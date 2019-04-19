#include "DXUT.h"
#include "CTransform2D.h"


CTransform2D::CTransform2D(lp_gameobject owner_obj)
	: CBaseTransform(owner_obj)
{

}

CTransform2D::~CTransform2D()
{
	if (m_pGameObject->GetTransform() == this)
		m_pGameObject->SetTransform(null);
}

void CTransform2D::Init()
{
}

void CTransform2D::Update()
{
}

void CTransform2D::Destroy()
{
}

void CTransform2D::SetTransform()
{
	D3DXMATRIX S, R, T;

	D3DXMatrixScaling(&S, m_vScale.x, m_vScale.y, m_vScale.z);

	D3DXMatrixRotationZ(&R, D3DXToRadian(m_vRotation.z));

	D3DXMatrixTranslation(&T, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = S * R * T;

	RESOURCE.GetSprite()->SetTransform(&m_matWorld);
}
