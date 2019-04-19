#include "DXUT.h"

SMultiTexture::~SMultiTexture()
{
	for (auto iter : m_textureContainer) SAFE_DELETE(iter);
}

SCustomTexture * SMultiTexture::GetImage(int count)
{
	if (count == -1)
		return this;
	else
		return m_textureContainer[count];
}

void SMultiTexture::AddImage(const std::string & path, int count)
{
	char sz[128] = "";

	for (int i = 1; i <= count; i++)
	{
		sprintf(sz, path.c_str(), i);

		SCustomTexture* tx = new SSingleTexture();
		tx->AddImage(sz);

		m_textureContainer.push_back(tx);
	}
}

size_t SMultiTexture::Size()
{
	return m_textureContainer.size();
}

