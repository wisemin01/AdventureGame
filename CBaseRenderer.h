#pragma once
#include "CBaseComponent.h"


class CBaseRenderer :
	public CBaseComponent
{
protected:

	std::function<void()> m_fpRenderBegin;
	std::function<void()> m_fpRenderEnd;

	INT m_renderLayer = 0;

public:
	CBaseRenderer(lp_gameobject owner_object, const object_id& obj_id = "");
	virtual ~CBaseRenderer();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	virtual void Set(const resource_id& rs_id) PURE;
	virtual void LoadSet(const resource_id& rs_id, const std::string& path, size_t count = 0) PURE;

	virtual void SetColor(int a, int r, int g, int b) PURE;

	void SetRenderBegin(std::function<void()> render_begin) { m_fpRenderBegin = render_begin; }
	void SetRenderEnd(std::function<void()> render_end) { m_fpRenderEnd = render_end; }

public:

	static bool CompareAsLayer(CBaseRenderer* render1, CBaseRenderer* render2)
	{
		return render1->m_renderLayer < render2->m_renderLayer;
	}

	static bool CompareAsY(CBaseRenderer* render1, CBaseRenderer* render2);
};

