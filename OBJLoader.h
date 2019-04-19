#pragma once

using namespace std;

struct SCacheEntry
{
	UINT index;
	SCacheEntry* pNext;
};

class COBJLoader
{
private:
	string OBJPath;

	vector<Vector3> Position;
	vector<Vector2> TexCoord;
	vector<Vector3> Normal;

	vector<VERTEX>  Vertex;
	vector<DWORD> Index;
	vector<DWORD> Attribute;

	vector <SCacheEntry*> VertexCache;

	string mtlPath;

	void CreateMesh(LPD3DXMESH * mesh);
	void MtlLoad(const string & mtlName, Mesh * mesh, const string & mapPath = "None");
	DWORD AddVertex(UINT hash, VERTEX* pVertex);
public:
	COBJLoader();
	~COBJLoader();

	void OBJLoad(Mesh * mesh, const string & objPath, const string & mapPath = "None");
};

