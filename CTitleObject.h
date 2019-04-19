#pragma once
#include "CBaseComponent.h"

class CSpriteRenderer;
class CTitleObject :
	public CBaseComponent
{
	CSpriteRenderer * m_pSpriteRenderer = null;

	lp_gameobject m_pHuman[2] = { null,null };
	lp_gameobject m_pTitle = null;

	bool m_bIsSummonHuman = false;
	bool m_bIsSummonTitle = false;

	int m_iDeleteRigidCount = 0;

	int m_titleAlpha;

public:
	CTitleObject(lp_gameobject owner_obj);
	virtual ~CTitleObject();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

};

