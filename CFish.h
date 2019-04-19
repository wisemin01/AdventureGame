#pragma once
#include "CBaseComponent.h"

enum class FISH_STATE
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

class CFish :
	public CBaseComponent
{
private:
	CSpriteRenderer	* m_pSpriteRenderer = null;
	CStateMachine	* m_pStateMachine	= null;
	CRigidbody2D	* m_pRigidbody		= null;
	CLifeMachine		* m_pLifeMachine = null;

	texture * m_pTexturesByState[(INT)FISH_STATE::END];

	float m_fEndLeft = 0;
	float m_fEndRight = 0;

	float m_fSpeed = 2;

	int m_iXDir = -1;
	int m_iYBaseLine = 0;
	int m_iYJumpLine = 0;

	float m_fAttackTime;

	bool m_bIsAttack = false;
	bool m_bIsUp = true;
public:
	CFish(lp_gameobject owner_obj);
	virtual ~CFish();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void SetTexturesByState();
	void SetupStateMachine();

	void OnCollision(CCollider2D* other);

public:
	void Set(float x, float y, float left_end, float right_end);
};

