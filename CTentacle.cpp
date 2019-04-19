#include "DXUT.h"
#include "CTentacle.h"

#include "CTransform2D.h"
#include "CSpriteRenderer.h"
#include "CCollider2D.h"
#include "CLifeMacine.h"

CTentacle::CTentacle(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj)
{
}


CTentacle::~CTentacle()
{
}

void CTentacle::Init()
{
	m_fEndTime = GetNowTime() + 1000;
	m_bIsRegister = false;
}

void CTentacle::Update()
{
	if (m_fEndTime - 300 <= GetNowTime())
		lerp(GO.GetTransform()->GetPosition(), *GO.GetTransform()->GetPosition(), m_vEndPos, GetDeltaTime() * 10);

	if (!m_bIsRegister && m_fEndTime <= GetNowTime())
	{
		GO.GC(CCollider2D)->Register();
		m_bIsRegister = true;
	}
	else if(m_bIsRegister && m_fEndTime + 50 <= GetNowTime())
	{
		GO.Kill();
	}
}

void CTentacle::Destroy()
{

}

void CTentacle::Setup(Vector3 start_pos, int flip)
{
	GO.AC(CTransform2D)->SetPosition(start_pos + Vector3(0, -100, 0));
	GO.AC(CSpriteRenderer)->Set("TENTACLE");
	GO.AC(CCollider2D)->SetRangeAsImage();

	GO.GC(CCollider2D)->AddOnCollisionFunc(COLLISION_FUNC(other)
	{
		if (other->GetGameObject()->GetObjectID() == "PLAYER")
			OnPlayer(other->GetGameObject());
	});
	GO.GC(CCollider2D)->SetIsTrigger(true);
	GO.GC(CSpriteRenderer)->SetRenderLayer(5);
	GO.GC(CSpriteRenderer)->Flip(flip);

	m_vEndPos = start_pos;
}

void CTentacle::OnPlayer(CGameObject * other)
{
	CLifeMachine* life = other->GC(CLifeMachine);

	if (life)
	{
		life->OnAttack(2);
		GO.Kill();
	}
}
