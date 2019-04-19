#pragma once
#include "CSingleton.h"
#include "OBJLoader.h"

class CResourceManager :
	public CSingleton<CResourceManager>
{
private:

	std::unordered_map<resource_id, Mesh*> m_pMeshContainer;
	std::unordered_map<resource_id, texture*> m_pTextureContainer;

	COBJLoader m_OBJloader;

	LPD3DXSPRITE m_pSprite = null;

	INT m_spriteBeginMode = 0;

public:
	CResourceManager();
	virtual ~CResourceManager();

	Mesh* AddMesh(const resource_id& id, const std::string& path);
	Mesh* FindMesh(const resource_id& id);

	texture* AddTexture(const resource_id& id, const std::string& path, int count = 0);
	texture* FindTexture(const resource_id& id, int count = -1);

	void SpriteBegin(INT begin_type = D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);
	void SpriteEnd();

	void Clear();

	LPD3DXSPRITE& GetSprite() { return m_pSprite; }

	void SetTransformAsSprite(Vector3 scale, Vector3 rotation, Vector3 position);
};

#define RESOURCE (*CResourceManager::Instance())