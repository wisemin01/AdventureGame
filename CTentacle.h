#pragma once
#include "CBaseComponent.h"
class CTentacle :
	public CBaseComponent
{
private:
	float m_fEndTime;
	bool m_bIsRegister;

	Vector3 m_vEndPos;
public:
	CTentacle(lp_gameobject owner_obj);
	virtual ~CTentacle();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void Setup(Vector3 start_pos, int flip = 1);

	void OnPlayer(CGameObject* other);
};

