#include "DXUT.h"
#include "CWaveRenderer.h"

#include "CTransform2D.h"
#include "CCollider2D.h"
#include "CRigidbody2D.h"

#include <random>

#define NUM_BACKGROUND_WAVES			(7)
#define BACKGROUND_WAVE_MAX_HEIGHT		(6)
#define BACKGROUND_WAVE_COMPRESSION		(1.f / 10.f)

// Èû Àü´Þ
#define SPRING_CONSTANT					(0.0095f)
#define SPRING_CONSTANT_BASELINE		(0.0095f)
// °¨¼è
#define DAMPING							(0.985f)
// ½Ã¹Ä·¹ÀÌ¼Ç È½¼ö
#define ITERATIONS						(3)

#define SPACING							(1.8f)

CWaveRenderer::CWaveRenderer(lp_gameobject owner_obj)
	: CSpriteRenderer(owner_obj, "WATER")
{
}


CWaveRenderer::~CWaveRenderer()
{
}

void CWaveRenderer::Init()
{
	GO.AC(CTransform2D);
	GO.SetObjectTag("WATER");
}

void CWaveRenderer::Update()
{
	UpdateWavePoints();

	sin_offset_time += GetDeltaTime() * 10.f;

	if (sin_offset_time >= 100)
		sin_offset_time -= 100;

	//m_targetContainer.clear();

	float top = y_offset - pixel_height / 2.f;
}

void CWaveRenderer::Render()
{
	RESOURCE.SpriteBegin(m_renderType);

	for (auto iter : m_pointContainer)
	{
		if (iter.x >= CAMERA.GetPosition().x + screen_width / 2.f + pixel_width
			|| iter.x <= CAMERA.GetPosition().x - screen_width / 2.f - pixel_width)
			continue;

		RESOURCE.SetTransformAsSprite(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(iter.x, iter.y + sin(iter.x * 0.075f + sin_offset_time) * 3, 0));

		texture* _draw = m_pTexture->GetImage(m_frame.current);

		Vector3 center;

		center.x = _draw->tInfo.Width / 2.f;
		center.y = _draw->tInfo.Height / 2.f;
		center.z = 0;

		RESOURCE.GetSprite()->Draw(_draw->pTexture, m_pCutRect,
			&center, null, m_color);
	}
}

void CWaveRenderer::Destroy()
{
}

void CWaveRenderer::SetUp(Vector2 start_pos, int vertex_count, const resource_id & wave_id, int layer)
{
	Set(wave_id);

	pixel_width = (float)m_pTexture->GetImage(0)->tInfo.Width - SPACING;
	pixel_height = (float)m_pTexture->GetImage(0)->tInfo.Height;

	y_offset = start_pos.y;
	m_vStartPos = Vector3(start_pos.x, start_pos.y, 0);
	sin_offset_time = 0;

	for (int i = 0; i < vertex_count; i++)
	{
		m_pointContainer.push_back(WavePoint2D(start_pos.x + i * pixel_width, start_pos.y));
	}

	m_pointCount = m_pointContainer.size();

	int half = (int)m_pointCount / 2;

	auto col = GO.AC(CCollider2D);

	col->SetRange(-pixel_width * half, -pixel_height / 2,
		pixel_width * half,
		pixel_height / 2);

	col->SetIsStaticCollider(true);
	col->SetPushPower(0.0f);
	col->SetFrictionalForce(0.13f, 0.13f);
	col->Register();

	float w = col->GetWidth();
	float h = col->GetHeight();

	m_renderLayer = layer;

	InterlockToCollider();

	GO.GetTransform()->SetPosition(start_pos.x + pixel_width * half, start_pos.y);
}

void CWaveRenderer::ResetWater(Vector2 new_position, int vertex_count)
{
	y_offset = new_position.y;
	m_vStartPos = Vector3(new_position.x, new_position.y, 0);
	sin_offset_time = 0;

	m_pointContainer.clear();

	for (int i = 0; i < vertex_count; i++)
	{
		m_pointContainer.push_back(WavePoint2D(new_position.x + i * pixel_width, new_position.y));
	}

	m_pointCount = m_pointContainer.size();

	int half = (int)m_pointCount / 2;

	auto col = GO.GC(CCollider2D);

	col->SetRange(-pixel_width * half, -pixel_height / 2,
		pixel_width * half,
		pixel_height / 2);

	GO.GetTransform()->SetPosition(new_position.x + pixel_width * half, new_position.y);
}

float CWaveRenderer::GetRandom()
{
	random_device rn;

	mt19937_64 rnd(rn());

	uniform_real_distribution<float> range(0, 0.99f);

	return range(rnd);
}

void CWaveRenderer::UpdateWavePoints() {

	for (int i = 0; i < ITERATIONS; i++) {

		for (int n = 0; n < m_pointCount; n++) {

			WavePoint2D &p = m_pointContainer[n];

			// force to apply to this point
			float force = 0;

			// forces caused by the point immediately to the left or the right
			float forceFromLeft = 0, forceFromRight = 0;

			if (n == 0) { // wrap to left-to-right
				float dy = m_pointContainer[m_pointCount - 1].y - p.y;
				forceFromLeft = SPRING_CONSTANT * dy;
			}
			else { // normally
				float dy = m_pointContainer[n - 1].y - p.y;
				forceFromLeft = SPRING_CONSTANT * dy;
			}
			if (n == m_pointCount - 1) { // wrap to right-to-left
				float dy = m_pointContainer[0].y - p.y;
				forceFromRight = SPRING_CONSTANT * dy;
			}
			else { // normally
				float dy = m_pointContainer[n + 1].y - p.y;
				forceFromRight = SPRING_CONSTANT * dy;
			}

			// Also apply force toward the baseline
			float dy = y_offset - p.y;
			float forceToBaseline = SPRING_CONSTANT_BASELINE * dy;

			// Sum up forces
			force += forceFromLeft + forceFromRight + forceToBaseline;

			// Calculate acceleration
			float acceleration = force / p.mass;

			// Apply acceleration (with damping)
			p.spd.y = DAMPING * p.spd.y + acceleration;
			
			// Apply speed
			p.y = p.y + p.spd.y;
		}
	}
}

void CWaveRenderer::EmpowerToWave(Vector3 position, float mess, float width)
{
	float real_width = width / pixel_width;

	int x = (int)(position.x - m_vStartPos.x) / (int)pixel_width;

	int devide_width = (INT)real_width / 2;

	float pic = mess / real_width;

	for (int i = -devide_width; i <= devide_width; i++) {

		int target_x = i + x;

		if (target_x >= 0 && target_x < m_pointCount)
			m_pointContainer[target_x].spd.y += mess - pic * abs(i);
	}
}

void CWaveRenderer::InterlockToCollider()
{
	CCollider2D* collider2D = GO.GC(CCollider2D);

	if (collider2D)
	{
		collider2D->AddOnCollisionFunc([&](CCollider2D* other) {
			
			if (other->GetIsTrigger())
				return;

			auto find = m_targetContainer.find(other->GetGameObject());

			if (find == m_targetContainer.end()) {

				m_targetContainer.insert(make_pair(other->GetGameObject(), GetNowTime() + 1000));
				Vector3 pos = *other->GetGameObject()->GetTransform()->GetPosition();

				pos.y += (other->GetHeight() * other->GetGameObject()->GetTransform()->GetScale()->y) / 2;

				CRigidbody2D* rigidbody = other->GetGameObject()->GC(CRigidbody2D);

				Vector3 this_pos = *GO.GetTransform()->GetPosition();

				float empower_width = other->GetWidth() * other->GetGameObject()->GetTransform()->GetScale()->x;
				pos.x -= empower_width / 2.f;

				if (rigidbody) {

					float gap = abs(pos.y - (this_pos.y - (pixel_height / 2)));

					if (gap <= 40)
					{
						EmpowerToWave(pos, rigidbody->GetMess() * rigidbody->GetVelocity().y * 0.001, empower_width);
						SOUND.DuplicatePlay("WATER");
					}
				}
			}
			else
			{
				if (find->second <= GetNowTime())
				{
					m_targetContainer.erase(find);
				}
			}
		});
	}
}
