#pragma once
#include "CSpriteRenderer.h"


class CScrollBackground :
	public CSpriteRenderer
{
private:
	INT m_iScroll;
	INT m_iSpeed;
	INT m_iWidth;
public:
	CScrollBackground(lp_gameobject owner_obj);
	virtual ~CScrollBackground();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void Set(const resource_id& text, int speed);
};

