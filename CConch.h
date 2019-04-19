#pragma once
#include "CBaseComponent.h"

enum class CONCH_STATE
{
	WALK,
	ATTACK,
	DEATH,
	END
};

class CTransform2D;
class CRigidbody2D;
class CStateMachine;
class CSpriteRenderer;
class CLifeMachine;

class CConch :
	public CBaseComponent
{
protected:
	CSpriteRenderer * m_pSpriteRenderer = null;
	CStateMachine * m_pStateMachine = null;
	CRigidbody2D * m_pRigidbody = null;
	CLifeMachine *  m_pLifeMacine = null;

	texture * m_pTexturesByState[(INT)CONCH_STATE::END];

	float m_fEndLeft = 0;
	float m_fEndRight = 0;

	float m_fSpeed = 4;

	int m_iXDir = -1;

public:
	CConch(lp_gameobject owner_obj);
	virtual ~CConch();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Set(float x, float y);

private:
	void SetTexturesByState();

	void SetupStateMachine();

	void OnCollision(CCollider2D* other);
};

