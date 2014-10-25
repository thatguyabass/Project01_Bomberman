//----------------------------
//textureclass.cpp
//----------------------------

#include "textureclass.h"

Texture::Texture()
{
	m_pTexture = 0;
}

Texture::Texture(const Texture& src)
{

}

Texture::~Texture()
{
	Shutdown();
}

bool Texture::Initialize(ID3D11Device* device, const wstring textureName)
{
	HRESULT result;

	//Load the texture in
	result = D3DX11CreateShaderResourceViewFromFile(device, textureName.c_str(), NULL, NULL, &m_pTexture, NULL);
	if(FAILED(result))
		return false;

	return true;
}

void Texture::Shutdown()
{
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_pTexture;
}