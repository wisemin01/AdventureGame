#include "DXUT.h"
#include "CSpriteRenderer.h"


CSpriteRenderer::CSpriteRenderer(lp_gameobject owner_obj, const object_id& obj_id)
	: CBaseRenderer(owner_obj, obj_id), m_color(0xffffffff)
{
}

CSpriteRenderer::~CSpriteRenderer()
{
}

void CSpriteRenderer::Init()
{
	m_frame.SetFrame(0, 0, 0);
}

void CSpriteRenderer::Update()
{
	m_frame.OnFrame();
}

void CSpriteRenderer::Render()
{
	RESOURCE.SpriteBegin(m_renderType);

	Vector3 scale = *GO.GetTransform()->GetScale();
	scale.x *= m_iFlip;

	CBaseTransform::SetTransformDefault(scale,
		*GO.GetTransform()->GetPosition() + m_vOffset,
		*GO.GetTransform()->GetRotation());

	texture* _draw = m_pTexture->GetImage(m_frame.current);

	Vector3 center;

	center.x = _draw->tInfo.Width / 2.f;
	center.y = _draw->tInfo.Height / 2.f;
	center.z = 0;

	RESOURCE.GetSprite()->Draw(_draw->pTexture, m_pCutRect,
		&center, null, m_color);
}

void CSpriteRenderer::Destroy()
{
	ReleaseCutRect();
}

void CSpriteRenderer::Set(const resource_id & rs_id)
{
	m_pTexture = RESOURCE.FindTexture(rs_id, -1);
}

void CSpriteRenderer::LoadSet(const resource_id & rs_id, const std::string & path, size_t count)
{
	m_pTexture = RESOURCE.AddTexture(rs_id, path, count);
}

void CSpriteRenderer::RenderFrame(int frame)
{
	RESOURCE.SpriteBegin(m_renderType);

	Vector3 scale = *GO.GetTransform()->GetScale();
	scale.x *= m_iFlip;

	CBaseTransform::SetTransformDefault(scale,
		*GO.GetTransform()->GetPosition() + m_vOffset,
		*GO.GetTransform()->GetRotation());

	texture* _draw = m_pTexture->GetImage(frame);

	Vector3 center;

	center.x = _draw->tInfo.Width / 2.f;
	center.y = _draw->tInfo.Height / 2.f;
	center.z = 0;

	RESOURCE.GetSprite()->Draw(_draw->pTexture, m_pCutRect,
		&center, null, m_color);
}

void CSpriteRenderer::SetByTexture(texture * text)
{
	m_pTexture = text;
}

void CSpriteRenderer::SetColor(int a, int r, int g, int b)
{
	m_color = D3DCOLOR_ARGB(a, r, g, b);
}

void CSpriteRenderer::SetFrame(int start, int end, float delay)
{
	m_frame.SetFrame(start, end, delay);
}

void CSpriteRenderer::SetFrame(int start, int end)
{
	m_frame.SetFrame(start, end);
}

void CSpriteRenderer::UseCutRect(float left, float top, float right, float bottom)
{
	if (!m_pCutRect)
		m_pCutRect = new RECT;

	SetRect(m_pCutRect, left, top, right, bottom);
}

void CSpriteRenderer::ReleaseCutRect()
{
	SAFE_DELETE(m_pCutRect);
}
