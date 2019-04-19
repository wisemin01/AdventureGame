#pragma once
#include "CBaseComponent.h"

enum class OCTOPUS_STATE
{
	DETEXTION,
	ATTACK1,
	ATTACK2,
	DEATH,
	END
};

class CTransform2D;
class CRigidbody2D;
class CCollider2D;
class CStateMachine;
class CSpriteRenderer;
class CLifeMachine;

class COctopus :
	public CBaseComponent
{
private:
	CSpriteRenderer * m_pSpriteRenderer = null;
	CStateMachine	* m_pStateMachine = null;
	CRigidbody2D	* m_pRigidbody = null;
	CLifeMachine	* m_pLifeMachine = null;
	CCollider2D		* m_pCollider2D = null;

	texture * m_pTexturesByState[(INT)OCTOPUS_STATE::END];

	float m_fDetectionTime = 0;
	bool m_bIsEndAttack = false;

	Vector3 m_vGageBarPos;

public:
	COctopus(lp_gameobject owner_obj);
	virtual ~COctopus();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(float x, float y, float hp);

private:

	void SetTexturesByState();

	void SetupStateMachine();

	void OnCollision(CCollider2D* other);
};

