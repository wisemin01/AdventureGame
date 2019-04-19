#include "DXUT.h"
#include "CCoconut.h"

#include "CCollider2D.h"
#include "CLifeMacine.h"

CCoconut::CCoconut(lp_gameobject owner_obj)
	: CBullet(owner_obj)
{

}


CCoconut::~CCoconut()
{
}

void CCoconut::Init()
{
}

void CCoconut::Update()
{
	CBullet::Update();
}

void CCoconut::Destroy()
{
}

void CCoconut::Setup(Vector3 position, Vector2 diretion)
{
	CBullet::Setup(position, diretion, "COCONUT" + std::to_string(GetIntRandom(1, 2)), 20, 1);
	m_pCollider->FuncClear();
	m_pCollider->AddOnCollisionFunc([&](CCollider2D* other) {OnCollision(other); });
}

void CCoconut::OnCollision(CCollider2D * other)
{
	if (other->GetGameObject()->GetObjectID() == "PLAYER")
	{
		CLifeMachine* life = other->GetGameObject()->GC(CLifeMachine);

		if (life)
		{
			GO.Kill();

			life->OnAttack(m_fDamage);
			//if (m_fSoundTime <= GetNowTime()) {
			//	SOUND.DuplicatePlay("HIT");
			//	m_fSoundTime = GetNowTime() + 200;
			//}

			return;
		}
	}
}
