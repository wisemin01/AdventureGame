#pragma once

using texture = struct SCustomTexture
{
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO tInfo;

	SCustomTexture() {}
	virtual ~SCustomTexture() {}

	virtual void AddImage(const std::string& path, int count = 0) PURE;
	virtual SCustomTexture* GetImage(int count = -1) PURE;

	virtual size_t Size() PURE;
};

using single_texture = struct SSingleTexture : public SCustomTexture
{
	SSingleTexture() {}
	virtual ~SSingleTexture() { SAFE_RELEASE(pTexture); }

	virtual void AddImage(const std::string& path, int count = 0) override
	{
		D3DXCreateTextureFromFileExA(DXUTGetD3D9Device(), path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 0,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(10, 10, 10), &tInfo, 0, &pTexture);
	}
	virtual SCustomTexture* GetImage(int count = -1) override
	{
		return this;
	}
	virtual size_t Size() { return 1; }
};

using multi_texture = struct SMultiTexture 
	: public SCustomTexture
{
	std::vector<SCustomTexture*> m_textureContainer;

	SMultiTexture() {}
	virtual ~SMultiTexture();

	virtual SCustomTexture* GetImage(int count = -1) override;

	virtual void AddImage(const std::string& path, int count = 0) override;
	
	virtual size_t Size();
};
