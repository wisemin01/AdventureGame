#include "DXUT.h"
#include "CTitleObject.h"

#include "CSpriteRenderer.h"
#include "CTransform2D.h"
#include "CCollider2D.h"
#include "CRigidbody2D.h"

#include "CStateMachine.h"

#include "CStage1.h"

CTitleObject::CTitleObject(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj)
{
}


CTitleObject::~CTitleObject()
{
}

void CTitleObject::Init()
{
	GO.AC(CTransform2D)->SetPosition(screen_width / 2.f, screen_height / 2.f);
	m_pSpriteRenderer = GO.AC(CSpriteRenderer); 
	m_pSpriteRenderer->Set("STAR");
	m_pSpriteRenderer->SetFrame(0, 4, 300);
	m_pSpriteRenderer->SetRenderLayer(1);

	m_iDeleteRigidCount = 0;
	m_titleAlpha = 0;

	auto bg = ENTITY.CreateDefault("BACKGROUND");
	bg->AC(CSpriteRenderer)->Set("BACKGROUND");
	bg->AC(CTransform2D)->SetPosition(screen_width / 2.f, screen_height / 2.f);

}

void CTitleObject::Update()
{
	if (!m_bIsSummonHuman && m_pSpriteRenderer->GetFrame()->current == 4)
	{
		m_bIsSummonHuman = true;

		int count = 0;

		for (auto &iter : m_pHuman)
		{
			iter = ENTITY.CreateDefault("HUMAN");

			iter->AC(CTransform2D)->SetPosition(screen_width / 2.f, screen_height * (count + 1));
			iter->AC(CSpriteRenderer)->SetByTexture(RESOURCE.FindTexture("HUMAN", count));
			iter->GC(CSpriteRenderer)->SetRenderLayer(4 - count);
			iter->AC(CRigidbody2D)->AddForce(0, -1500 - count * 1000);
			iter->GC(CRigidbody2D)->SetMess(2.0f);

			count++;
		}
	}

	if (m_bIsSummonHuman)
		for (auto &iter : m_pHuman)
		{
			CRigidbody2D* rigid = iter->GetComponent<CRigidbody2D>();

			if (!rigid)
				continue;

			if (rigid->GetVelocity().y > 0)
			{
				if (iter->GetTransform()->GetPosition()->y >= screen_height / 2.f)
				{
					iter->DeleteComponent<CRigidbody2D>();
					m_iDeleteRigidCount++;
				}
			}
		}


	if (!m_bIsSummonTitle && m_iDeleteRigidCount == 2)
	{
		m_bIsSummonTitle = true;

		m_pTitle = ENTITY.CreateDefault("TITLE");
		
		m_pTitle->AC(CTransform2D)->SetPosition(292 / 2.f + 50, 176 / 2.f + 50);
		m_pTitle->AC(CSpriteRenderer)->SetByTexture(RESOURCE.FindTexture("TITLE"));
		m_pTitle->GC(CSpriteRenderer)->SetRenderLayer(5);
	}

	if (m_bIsSummonTitle)
	{
		m_pTitle->GC(CSpriteRenderer)->SetColor(m_titleAlpha, 255, 255, 255);
		lerp(&m_titleAlpha, m_titleAlpha, 255, GetDeltaTime() * 2);

		if (INPUT.GetKeyDown(VK_LBUTTON))
		{
			SCENE.ChangeScene<CStage1>();
		}
	}
}

void CTitleObject::Destroy()
{
}
