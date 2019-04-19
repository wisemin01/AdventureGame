#include "DXUT.h"
#include "CBaseRenderer.h"

#include "CBaseTransform.h"

CBaseRenderer::CBaseRenderer(lp_gameobject owner_object, const object_id& obj_id)
	: CBaseComponent(owner_object, obj_id)
{
	m_pGameObject->SetRenderer(this);
}

CBaseRenderer::~CBaseRenderer()
{
	if (m_pGameObject->GetRenderer() == this)
		m_pGameObject->SetRenderer(null);
}

void CBaseRenderer::Init()
{
}

void CBaseRenderer::Update()
{
}

void CBaseRenderer::Render()
{
}

void CBaseRenderer::Destroy()
{
}

bool CBaseRenderer::CompareAsY(CBaseRenderer * render1, CBaseRenderer * render2)
{
	if (render1->m_renderLayer == render2->m_renderLayer)
		return render1->m_pGameObject->GetTransform()->GetPosition()->y <
		render2->m_pGameObject->GetTransform()->GetPosition()->y;
	else
		return false;
}
