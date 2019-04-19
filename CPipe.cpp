#include "DXUT.h"
#include "CPipe.h"

#include "CTransform2D.h"
#include "CCollider2D.h"
#include "CSpriteRenderer.h"

CPipe::CPipe(lp_gameobject owner_obj)
	: CObstacle(owner_obj)
{
}


CPipe::~CPipe()
{
}

void CPipe::Init()
{
}

void CPipe::Update()
{
}

void CPipe::Destroy()
{
}

void CPipe::Setup(PIPE_TYPE type, float x, float y, std::function<void(lp_gameobject)>)
{
	m_eType = type;

	GO.AC(CTransform2D)->SetPosition(x, y);
	GO.AC(CSpriteRenderer)->Set("PIPE");

	Vector2 size = GO.GC(CSpriteRenderer)->GetImageSize();

	m_fWidth = size.x;
	m_fHeight = size.y;

	auto col = GO.AC(CCollider2D);

	col->SetRange(m_fWidth, m_fHeight);
	col->Register();
	col->SetIsStaticCollider(true);
	col->AddOnCollisionFunc([&](CCollider2D* other) { OnCollision(other); });
	col->SetFrictionalForce(0.03f, 0.3f);
	col->SetPushPower(300);
}
