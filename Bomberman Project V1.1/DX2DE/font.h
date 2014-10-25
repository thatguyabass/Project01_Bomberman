//------------------------------------------
//font.h
//------------------------------------------
//Font class handles loading and containing the data used by other classes to display text on screen.

#ifndef _FONT_CLASS_H_
#define _FONT_CLASS_H_

#include<D3D11.h>

#include<Windows.h>
#include<xnamath.h>
#include<fstream>
#include<string>
#include<memory>

using namespace std;

#include "textureclass.h"

//------------------------------------------------------------------------------

class Font
{
public:
	Font();
	Font(const Font&);
	~Font();

	bool Initialize(ID3D11Device*, char*, wstring);

	ID3D11ShaderResourceView* GetTexture();

	//Build the Text Vertex Objects
	void BuildVertexArray(void*, string, float, float, XMFLOAT4, float = 1.0f);

	//String Length Calculations
	int GetCharacterWidth(char);
	int GetCharacterHeight(char);
	XMFLOAT2 GetTextDimension(string, float = 1.0f);

private:
	//Values gathered from the Font Text File to be used during Vertex Creation and passed to the texture shader
	struct FontType
	{
		float left, right, top, bottom;
		int width, height;
	};
		
	//Variables used to communicate the values of this object to the Texture Shader
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};
	
	//Methods
	void Shutdown();
	//Read the data from the text file to be used in the vertex creation
	bool LoadFontData(char*);
	void ReleaseFontData();

	bool LoadTexture(ID3D11Device*, wstring);
	void ReleaseTexture();

	//Variables
	FontType* m_pFont;//Array
	shared_ptr<Texture> m_pTexture;

};

#endif//_FONT_CLASS_H_