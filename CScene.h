#pragma once
#include "CObject.h"
class CScene :
	public CObject
{
protected:
	bool m_bIsEndLoading = false;
public:
	CScene();
	virtual ~CScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	virtual void HandleMessage(lp_message msg) override;

	virtual void Load() PURE;

	void EndLoading() { m_bIsEndLoading = true; }
	bool IsEndLoading () { return m_bIsEndLoading; }
};