#pragma once

#include "texture.h"

class CObject;
class CGameObject;
class CMessage;
class CScene;
class CBaseComponent;
class CBaseRenderer;
class CBaseTransform;

using message			= CMessage;
using object			= CObject;
using gameobject		= CGameObject;
using scene				= CScene;
using component			= CBaseComponent;
using renderer			= CBaseRenderer;
using transform			= CBaseTransform;

using lp_void			= void * ;
using lp_object			= object * ;
using lp_gameobject		= gameobject * ;
using lp_message		= message * ;
using lp_scene			= scene * ;
using lp_component		= component * ;

using object_id			= std::string;
using object_tag		= std::string;
using component_id		= std::string;
using scene_id			= std::string;
using resource_id		= std::string;

using object_list		= std::list<lp_object>;
using lp_object_list	= object_list * ;
using gameobject_list	= std::list<lp_gameobject>;
using lp_gameobject_list= gameobject_list * ;
using render_list		= std::list<renderer*>;
using lp_render_list	= render_list * ;
using multi_map			= std::unordered_map<component_id, lp_component>;
using component_map		= multi_map;
using lp_component_map	= component_map * ;


using message_type		= enum EMessageType {
	eMsgDestroy,
	eMsgPlayerDeath
};

using is_live			= bool;
using is_enable			= bool;


using Vector2 = D3DXVECTOR2;
using Vector3 = D3DXVECTOR3;
using Vector4 = D3DXVECTOR4;

using Matrix = D3DXMATRIX;

struct VERTEX
{
	Vector3 pos;
	Vector3 normal;
	Vector2 tex;

	VERTEX(Vector3 _pos, Vector2 _tex, Vector3 _normal)
		:pos(_pos), tex(_tex), normal(_normal)
	{}
	VERTEX() {}
};

#define VERTEXFVF D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_TEX1


struct Material
{
	std::string mtlName;

	D3DMATERIAL9 material;
	texture * map = nullptr;

	float fAlpha = 1;
	float fIllum;
	float fNs;

	Material()
	{
		material.Power = 1;
	}
};

struct Mesh
{
	LPD3DXMESH lpD3DXMesh;
	std::vector<Material*> vMaterial;

	~Mesh()
	{
		for (auto Iter : vMaterial)
			SAFE_DELETE(Iter);

		vMaterial.clear();

		SAFE_RELEASE(lpD3DXMesh);
	}
};

struct Frame
{
	int current, start, end;
	unsigned long delay, frame_skip;

	bool OnFrame();
	
	void SetFrame(int start, int end, unsigned long delay);
	void SetFrame(int start, int end);
	void SetDelay(unsigned long delay) { Frame::delay = delay; }
};

using vertex_vector = std::vector<VERTEX>;
using index_vector = std::vector<unsigned short>;
using attribute_vector = std::vector<unsigned long>;
using material_vector = std::vector<Material>;



#define null nullptr

#define now_time		timeGetTime()
#define elapsed_time	DXUTGetElapsedTime()

#define GO (*m_pGameObject)