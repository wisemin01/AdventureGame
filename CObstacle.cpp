#include "DXUT.h"
#include "CObstacle.h"

#include "CTransform2D.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"

CObstacle::CObstacle(lp_gameobject owner_obj)
	: CBaseComponent(owner_obj, "OBSTACLE")
{
}


CObstacle::~CObstacle()
{
}

void CObstacle::Init()
{
	
}

void CObstacle::Update()
{

}

void CObstacle::Destroy()
{
}

void CObstacle::Set(const resource_id & id, float x, float y, Vector2 attenuation)
{
	GO.AC(CTransform2D)->SetPosition(x, y);
	GO.AC(CSpriteRenderer)->Set(id);

	Vector2 size = GO.GC(CSpriteRenderer)->GetImageSize();

	m_fWidth = size.x;
	m_fHeight = size.y;

	auto col = GO.AC(CCollider2D); 

	col->SetRange(m_fWidth, m_fHeight);
	col->Register();
	col->SetIsStaticCollider(true);
	col->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
	col->SetFrictionalForce(attenuation.x, attenuation.y);
	col->SetPushPower(300);
}

void CObstacle::SetByWoodBlock(int size, float x, float y, Vector2 attenuation)
{
	float width = size * 85;

	switch (size)
	{
	case 1:
		GO.AC(CSpriteRenderer)->Set("WOOD1");
		break;
	case 3:
		GO.AC(CSpriteRenderer)->Set("WOOD3");
		break;
	case 5:
		GO.AC(CSpriteRenderer)->Set("WOOD5");
		break;

	default: return;
	}

	Vector2 image_size = GO.GC(CSpriteRenderer)->GetImageSize();

	m_fWidth = image_size.x;
	m_fHeight = image_size.y;
	GO.AC(CTransform2D)->SetPosition(x, y - m_fHeight / 2.f + 35);
	GO.GC(CSpriteRenderer)->SetOffset(0, m_fHeight / 2.f - 35);

	auto col = GO.AC(CCollider2D);

	col->SetRange(-m_fWidth / 2.f, -35, m_fWidth / 2.f, 35);
	col->Register();
	col->SetIsStaticCollider(true);
	col->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
	col->SetFrictionalForce(attenuation.x, attenuation.y);
	col->SetPushPower(300);
}

void CObstacle::SetByFence(float x, float y, Vector2 attenuation)
{
	Set("FENCE", x, y, attenuation);
	m_pGameObject->GetComponent<CCollider2D>()->SetPassAble(true);
}

void CObstacle::OnCollision(CCollider2D * other)
{
	
}
