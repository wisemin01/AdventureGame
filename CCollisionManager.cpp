#include "DXUT.h"
#include "CCollisionManager.h"

#include "CCollider2D.h"

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
	m_colliderContainer.clear();
}

void CCollisionManager::CollisionCheck()
{
	for (auto iter : m_colliderContainer)
	{
		for (auto iter2 : m_colliderContainer)
		{
			if (iter == iter2)
			{
				continue;
			}

			RECT out;

			RECT _1RECT = iter->GetWorldRange();
			RECT _2RECT = iter2->GetWorldRange();

			if (IntersectRect(&out, &_1RECT, &_2RECT))
			{
				iter->OnCollision(iter2);
			}
		}
	}
}

void CCollisionManager::Register(CCollider2D * col)
{
	m_colliderContainer.push_back(col);
}

void CCollisionManager::UnRegister(CCollider2D * col)
{
	m_colliderContainer.remove(col);
}
