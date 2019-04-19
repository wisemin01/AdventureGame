#pragma once
#include "CSpriteRenderer.h"

struct WavePoint2D
	: public Vector2
{
	float mass = 1.0f;
	Vector2 spd = Vector2(0, 0);

	WavePoint2D(float x, float y) : Vector2(x, y) {}
};


class CWaveRenderer :
	public CSpriteRenderer
{
protected:
	std::vector<WavePoint2D> m_pointContainer;
	int m_pointCount;

	// A phase difference to apply to each sine
	float offset = 0;
	float y_offset = 500;

	float pixel_width = 0;
	float pixel_height = 0;

	float sin_offset_time = 0;

	Vector3 m_vStartPos;

	std::map<CGameObject*, float> m_targetContainer;

public:
	CWaveRenderer(lp_gameobject owner_obj);
	virtual ~CWaveRenderer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void SetUp(Vector2 start_pos, int vertex_count, const resource_id& wave_id, int layer = 0);

	void ResetWater(Vector2 new_position, int vertex_count);

private:
	float GetRandom(); 
	
	void UpdateWavePoints();

	void EmpowerToWave(Vector3 position, float mess, float width);

	void InterlockToCollider();
};

