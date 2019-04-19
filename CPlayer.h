#pragma once
#include "CBaseComponent.h"

enum class PLAYER_STATE
{
	IDLE = 0,
	WALK = 1,
	JUMP = 2,
	ATTACK = 3,
	DEATH = 4,
	END = 5
};

enum class PLAYER_ITEM_STATE
{
	DEFAULT = 0,
	_3DIRECTION = 1,
	NUCLEAR = 2,
	TARGET = 3,
	END = 4
};

class CRigidbody2D;
class CStateMachine;
class CSpriteRenderer;

class CPlayer :
	public CBaseComponent
{
private:
	CRigidbody2D	* m_pRigidbody		= null;
	CStateMachine	* m_pStateMachine	= null;
	CSpriteRenderer * m_pSpriteRenderer = null;

	texture* m_pTexturesByState[(INT)PLAYER_STATE::END][(INT)PLAYER_ITEM_STATE::END];

	bool m_bIsOnObject = true;
	bool m_bIsOnFrameMove = false;
	bool m_bIsJumpOnFrame = false;
	bool m_bIsAttackOnFrame = false;

	float m_fFootHeight;
	float m_fAttackDelay[(INT)PLAYER_ITEM_STATE::END];

	PLAYER_ITEM_STATE m_eItemState = PLAYER_ITEM_STATE::DEFAULT;

public:
	CPlayer(lp_gameobject owner);
	virtual ~CPlayer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void HandleMessage(lp_message msg);

	void OnCollision(CCollider2D* other);

	void Setup(float x, float y, float size);
private:
	void SetupStateMachine();
	void SetupCollider();
	void SetupRigidbody();

	void SetupTexturesByState();

	void JumpCheck();
	void AttackCheck();
	void MoveCheck();
	void ModeCheck();

	void ChangeMode(PLAYER_ITEM_STATE item_state);

	void Attack(PLAYER_ITEM_STATE item_state);

public:
	static Vector3 g_vReSpawn;

	static void SetReSpawnPos(Vector3 respawn) { g_vReSpawn = respawn;	}
	static Vector3  GetReSpawnPos() { return g_vReSpawn; }
};

