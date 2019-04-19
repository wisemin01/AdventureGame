#pragma once
#include "CBaseRenderer.h"


class CSpriteRenderer :
	public CBaseRenderer
{
protected:
	texture * m_pTexture = null;

	INT m_renderType = D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE;
	INT m_iFlip = 1;

	Frame m_frame;

	LPRECT m_pCutRect = null;

	DWORD m_color;

	Vector3 m_vOffset = Vector3(0, 0, 0);
public:
	CSpriteRenderer(lp_gameobject owner_obj, const object_id& obj_id = "");
	virtual ~CSpriteRenderer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Destroy() override;

	virtual void Set(const resource_id& rs_id) override;
	virtual void LoadSet(const resource_id& rs_id, const std::string& path, size_t count = 0) override;

	void RenderFrame(int frame);

	void SetByTexture(texture* text);

	virtual void SetColor(int a, int r, int g, int b) override;

	void SetFrame(int start, int end, float delay);
	void SetFrame(int start, int end);

	void SetRenderType(INT type) { m_renderType = type; }
	void SetRenderLayer(INT layer) { m_renderLayer = layer; }

	void Flip(INT direction_x) { m_iFlip = direction_x; }

	INT GetFlip() { return m_iFlip; }

	Frame* GetFrame() { return &m_frame; }
	const Vector2& GetImageSize() {
		if (m_pTexture)
			return Vector2(m_pTexture->GetImage(0)->tInfo.Width,
				m_pTexture->GetImage(0)->tInfo.Height);
		else return Vector2(0, 0);
	}

	std::tuple<float, float> GetImageSizeTuple()
	{
		if (m_pTexture) {
			return make_tuple(m_pTexture->GetImage(0)->tInfo.Width,
				m_pTexture->GetImage(0)->tInfo.Height);
		}
		else
		{
			return make_tuple(0, 0);
		}
	}

	Vector3* GetOffset() { return &m_vOffset; }
	void SetOffset(float x, float y, float z = 0.0f) { m_vOffset = Vector3(x, y, z); }

	LPRECT GetRect() { return m_pCutRect; }
	void UseCutRect(float left, float top, float right, float bottom);
	void ReleaseCutRect();
};

