#include "DXUT.h"
#include "CScrollBackground.h"

#include "CTransform2D.h"

CScrollBackground::CScrollBackground(lp_gameobject owner_obj)
	: CSpriteRenderer(owner_obj, "BACKGROUND")
{
}


CScrollBackground::~CScrollBackground()
{
}

void CScrollBackground::Init()
{
	GO.AC(CTransform2D)->SetPosition(screen_width / 2.f, screen_height / 2.f);
}

void CScrollBackground::Update()
{
	m_iScroll += (float)m_iSpeed * GetDeltaTime();
	m_iWidth = GetImageSize().x;

	GO.GetTransform()->GetPosition()->x = m_iScroll % m_iWidth;
}

void CScrollBackground::Render()
{
	float x = GO.GetTransform()->GetPosition()->x;

	CSpriteRenderer::Render();

	GO.GetTransform()->GetPosition()->x -= m_iWidth;

	CSpriteRenderer::Render();

	GO.GetTransform()->GetPosition()->x = x;
}

void CScrollBackground::Destroy()
{
}

void CScrollBackground::Set(const resource_id & text, int speed)
{
	CSpriteRenderer::Set(text);

	m_renderType = D3DXSPRITE_ALPHABLEND;
	m_renderLayer = -11;

	m_iSpeed = speed;
	m_iScroll = 0;
}
