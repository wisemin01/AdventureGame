#include "DXUT.h"
#include "CCollider2D.h"

#include "CSpriteRenderer.h"

CCollider2D::CCollider2D(lp_gameobject owner_obj)
	: CBaseCollider(owner_obj)
{
}


CCollider2D::~CCollider2D()
{
	UnRegister();
}

void CCollider2D::SetRange(float range)
{
	m_rcRange.left = -range / 2.f;
	m_rcRange.right = range / 2.f;
	m_rcRange.top = -range / 2.f;
	m_rcRange.bottom = range / 2.f;
}

void CCollider2D::SetRange(float width, float height)
{
	m_rcRange.left = -width / 2.f;
	m_rcRange.right = width / 2.f;
	m_rcRange.top = -height / 2.f;
	m_rcRange.bottom = height / 2.f;
}

void CCollider2D::SetRange(float left, float top, float right, float bottom)
{
	m_rcRange.left = left;
	m_rcRange.right = right;
	m_rcRange.top = top;
	m_rcRange.bottom = bottom;
}

void CCollider2D::SetRangeAsImage()
{
	float w, h;
	CSpriteRenderer* sprite_renderer = GO.GC(CSpriteRenderer);

	if (sprite_renderer)
	{
		std::tie(w, h) = sprite_renderer->GetImageSizeTuple();

		SetRange(w, h);
	}
}

RECT CCollider2D::GetWorldRange() const
{
	const Vector3* position = GO.GetTransform()->GetPosition();
	const Vector3* scale = GO.GetTransform()->GetScale();

	RECT ret = m_rcRange;

	ret.left = position->x + m_rcRange.left * scale->x;
	ret.right = position->x + m_rcRange.right * scale->x;
	ret.top = position->y + m_rcRange.top * scale->y;
	ret.bottom = position->y + m_rcRange.bottom * scale->y;

	return ret;
}

void CCollider2D::Register()
{
	COLLISION.Register(this);
}

void CCollider2D::UnRegister()
{
	COLLISION.UnRegister(this);
}

void CCollider2D::AddOnCollisionFunc(std::function<void(CCollider2D*)> pFunc)
{
	m_pOnCollisionList.push_back(pFunc);
}

void CCollider2D::FuncClear()
{
	m_pOnCollisionList.clear();
}

void CCollider2D::OnCollision(CCollider2D * other)
{
	for (auto iter : m_pOnCollisionList)
	{
		iter(other);
	}
}

void CCollider2D::Setup(float width, float height)
{
	SetRange(width, height);

	Register();
}

void CCollider2D::Setup(float left, float top, float right, float bottom)
{
	SetRange(left, top, right, bottom);

	Register();
}

void CCollider2D::Setup()
{
	SetRangeAsImage();

	Register();
}
