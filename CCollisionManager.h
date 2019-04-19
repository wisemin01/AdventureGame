#pragma once
#include "CSingleton.h"

class CCollider2D;
class CCollisionManager :
	public CSingleton<CCollisionManager>
{
private:
	std::list<CCollider2D*> m_colliderContainer;
public:
	CCollisionManager();
	virtual ~CCollisionManager();
	
	void CollisionCheck();

	void Register(CCollider2D* col);
	void UnRegister(CCollider2D* col);
};

#define COLLISION (*CCollisionManager::Instance())