#include "DXUT.h"
#include "CMeshRenderer.h"


CMeshRenderer::CMeshRenderer(lp_gameobject owner_obj)
	: CBaseRenderer(owner_obj)
{
}


CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::Init()
{
}

void CMeshRenderer::Update()
{
}

void CMeshRenderer::Render()
{
	if (m_fpRenderBegin)
		m_fpRenderBegin();

	m_pGameObject->GetTransform()->SetTransform();

	for (int i = 0; i < m_pMeshInfo->vMaterial.size(); ++i)
	{
		if (m_pMeshInfo->vMaterial[i]->map)
			GetD3D9Device()->SetTexture(0, m_pMeshInfo->vMaterial[i]->map->pTexture);
		else
			GetD3D9Device()->SetTexture(0, nullptr);

		GetD3D9Device()->SetMaterial(&m_pMeshInfo->vMaterial[i]->material);
		m_pMeshInfo->lpD3DXMesh->DrawSubset(i);
	}

	if (m_fpRenderEnd)
		m_fpRenderEnd();
}

void CMeshRenderer::Destroy()
{
}

void CMeshRenderer::Set(const resource_id & rs_id)
{
	m_pMeshInfo = RESOURCE.FindMesh(rs_id);
}

void CMeshRenderer::LoadSet(const resource_id & rs_id, const std::string & path, size_t count)
{
	m_pMeshInfo = RESOURCE.AddMesh(rs_id, path);
}

