#include "DXUT.h"
#include "CGageBar.h"

#include "CTransform2D.h"
#include "CSpriteRenderer.h"
#include "CLifeMacine.h"

CGageBar::CGageBar(lp_gameobject owner_obj)
	: CSpriteRenderer(owner_obj)
{
}


CGageBar::~CGageBar()
{
}

void CGageBar::Init()
{
}

void CGageBar::Update()
{
	if (!m_pTargetLifeMachine->GetGameObject()->IsLive())
	{
		GO.Kill();
	}

	GO.GetTransform()->SetPosition(*m_vTargetPos);

	float perc = (float)m_pTargetLifeMachine->GetHP() / (float)m_pTargetLifeMachine->GetMaxHP();

	tie(m_fWidth, m_fHeight) = GetImageSizeTuple();

	UseCutRect(0, 0, m_fWidth, m_fHeight);

	m_fWidth *= perc;

	lerp(&m_fLerpWidth, m_fLerpWidth, m_fWidth, GetDeltaTime() * 1.5f);
}

void CGageBar::Render()
{
	CSpriteRenderer::RenderFrame(0);

	UseCutRect(0, 0, m_fLerpWidth, m_fHeight);

	CSpriteRenderer::RenderFrame(1);

	UseCutRect(0, 0, m_fWidth, m_fHeight);

	CSpriteRenderer::RenderFrame(2);
}

void CGageBar::Destroy()
{
	CSpriteRenderer::Destroy();
}

void CGageBar::Setup(CGameObject* target_obj, Vector3 * target, Vector3 offset, const resource_id& rc_id)
{
	GO.AC(CTransform2D)->SetPosition(*target);

	m_pTargetLifeMachine = target_obj->GC(CLifeMachine);

	m_vTargetPos = target;
	Set(rc_id);
	SetOffset(offset.x, offset.y);

	m_fLerpWidth = GetImageSize().x;
}
