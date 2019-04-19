#pragma once
#include "CBaseRenderer.h"


class CMeshRenderer :
	public CBaseRenderer
{
private:
	Mesh * m_pMeshInfo;

public:
	CMeshRenderer(lp_gameobject owner_obj);
	virtual ~CMeshRenderer();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	virtual void Set(const resource_id& rs_id) override;
	virtual void LoadSet(const resource_id& rs_id, const std::string& path, size_t count = 0) override;

	virtual void SetColor(int a, int r, int g, int b) override {}
};

