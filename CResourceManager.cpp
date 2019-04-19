#include "DXUT.h"
#include "CResourceManager.h"

#include <fstream>
#include "OBJLoader.h"

CResourceManager::CResourceManager()
{
	D3DXCreateSprite(GetD3D9Device(), &m_pSprite);
}


CResourceManager::~CResourceManager()
{
	Clear();

	SAFE_RELEASE(m_pSprite);
}

Mesh * CResourceManager::AddMesh(const resource_id & id, const std::string & path)
{
	auto find = m_pMeshContainer.find(id);

	if (find == m_pMeshContainer.end())
	{
		Mesh* pNewMesh = new Mesh;
		m_OBJloader.OBJLoad(pNewMesh, path);

		m_pMeshContainer.insert(std::make_pair(id, pNewMesh));

		return pNewMesh;
	}

	return find->second;
}

Mesh * CResourceManager::FindMesh(const resource_id & id)
{
	auto find = m_pMeshContainer.find(id);

	if (find != m_pMeshContainer.end())
	{
		return find->second;
	}

	return nullptr;
}

texture* CResourceManager::AddTexture(const resource_id & id, const std::string & path, int count)
{
	auto find = m_pTextureContainer.find(id);

	console_log(id << " is on loading...");

	if (find == m_pTextureContainer.end())
	{
		texture* tx;

		if (count == 0)
			tx = new single_texture;
		else
			tx = new multi_texture;

		tx->AddImage(path, count);

		m_pTextureContainer.insert(std::make_pair(id, tx));

		return tx;
	}

	return find->second;
}

texture * CResourceManager::FindTexture(const resource_id & id, int count)
{
	auto find = m_pTextureContainer.find(id);

	if (find != m_pTextureContainer.end())
	{
		return find->second->GetImage(count);
	}

	return nullptr;
}

void CResourceManager::SpriteBegin(INT begin_type)
{
	if (m_spriteBeginMode == D3DXSPRITE_NONE)
	{
		m_spriteBeginMode = begin_type;
		m_pSprite->Begin(begin_type);
		return;
	}

	if (m_spriteBeginMode != begin_type) {

		m_spriteBeginMode = begin_type;
		m_pSprite->End();
		m_pSprite->Begin(begin_type);
		return;
	}
}

void CResourceManager::SpriteEnd()
{
	if (m_spriteBeginMode != D3DXSPRITE_NONE)
	{
		m_spriteBeginMode = D3DXSPRITE_NONE;
		m_pSprite->End();
	}
}

void CResourceManager::Clear()
{
	for (auto iter : m_pMeshContainer)
	{
		SAFE_DELETE(iter.second);
	}
	m_pMeshContainer.clear();

	for (auto iter : m_pTextureContainer)
	{
		SAFE_DELETE(iter.second);
	}
	m_pTextureContainer.clear();
}

void CResourceManager::SetTransformAsSprite(Vector3 scale, Vector3 rotation, Vector3 position)
{
	D3DXMATRIX S, R, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);

	D3DXMatrixRotationZ(&R, D3DXToRadian(rotation.z));

	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	D3DXMATRIX matWorld = S * R * T;

	RESOURCE.GetSprite()->SetTransform(&matWorld);
}
