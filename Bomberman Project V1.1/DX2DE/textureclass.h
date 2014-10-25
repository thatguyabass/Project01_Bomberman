//---------------------------------------
//textureclass.h
//---------------------------------------
//this class encapsulates the loading, unloading and accessing of a single texture (for each texture needed, an object of this class must instantiated)

#ifndef _TEXTURE_CLASS_H_
#define _TEXTURE_CLASS_H_

#include<D3D11.h>
#include<D3DX11tex.h>

#include<Windows.h>
#include<string>
using namespace std;

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, const wstring);

	ID3D11ShaderResourceView* GetTexture();

private:
	void Shutdown();
	ID3D11ShaderResourceView* m_pTexture;
};

#endif //_TEXTURE_CLASS_H_
