#include "DXUT.h"
#include "CSlowMoveBackground.h"

#include "CSpriteRenderer.h"
#include "CTransform2D.h"

CSlowMoveBackground::CSlowMoveBackground(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj, "BACKGROUND")
{

}


CSlowMoveBackground::~CSlowMoveBackground()
{
}

void CSlowMoveBackground::Init()
{

}

void CSlowMoveBackground::Update()
{
	float x = CAMERA.GetPosition().x * m_fSlowRed;

	GO.GetTransform()->GetPosition()->x = -x;
}

void CSlowMoveBackground::Destroy()
{
}

void CSlowMoveBackground::Set(const resource_id & id, float startX, float slow_red)
{
	auto renderer = GO.AC(CSpriteRenderer);
	GO.AC(CTransform2D)->SetPosition(startX, screen_height / 2);

	renderer->Set(id);
	renderer->SetRenderType(D3DXSPRITE_ALPHABLEND);
	renderer->SetRenderLayer(-10);

	m_fSlowRed = slow_red;

	m_fImageWidth = renderer->GetImageSize().x;
}

void CSlowMoveBackground::HandleMessage(lp_message msg)
{
	switch (msg->GetType())
	{
	case eMsgDestroy:
		GO.Kill();
		return;
	}
}
