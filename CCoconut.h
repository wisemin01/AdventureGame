#pragma once
#include "CBullet.h"


class CCoconut :
	public CBullet
{
public:
	CCoconut(lp_gameobject owner_obj);
	virtual ~CCoconut();

	virtual void Init() override;

	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(Vector3 position, Vector2 diretion);

	virtual void OnCollision(CCollider2D* other);
};

