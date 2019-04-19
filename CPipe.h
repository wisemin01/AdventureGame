#pragma once
#include "CObstacle.h"

enum class PIPE_TYPE
{
	OBSTACLE,
	SPAWN_THINGS,
	PLAYER_PATH
};

class CPipe :
	public CObstacle
{
protected:
	PIPE_TYPE m_eType = PIPE_TYPE::OBSTACLE;
public:
	CPipe(lp_gameobject owner_obj);
	virtual ~CPipe();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(PIPE_TYPE type, float x, float y, std::function<void(lp_gameobject)>);
};

