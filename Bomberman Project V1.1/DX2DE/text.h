//---------------------------------------
//text.h
//---------------------------------------
//Handles Rendering text on screen

#ifndef _TEXT_H_
#define _TEXT_H_

#include "textureshader.h"
#include "font.h"
#include "Util.h"

#include<memory>

//----------------------------------------------------------------------------------------------------------------------------------------

class Text
{
public:
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(ID3D11Device*, shared_ptr<Font>, string, int, XMFLOAT2, XMFLOAT4, float fontScale = 1);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);	
	void Update();

	//Change the text, position, color and/or scale
	void Change(string, XMFLOAT2, XMFLOAT4, float fontScale = 1);

	//Offset Access Metheds - Adjust the position
	void SetOffset(XMFLOAT2);
	XMFLOAT2 GetOffset();

	//Text Access Methods
	void SetText(string);
	string GetText();
	
	//Movement Methods
	void Move(float d, XMFLOAT2);
	void AdjustPos(XMFLOAT2);

	//Local Position Access Method
	void SetLocal(XMFLOAT2);
	XMFLOAT2 GetLocal();

	//World Position Access Method
	void SetPosition(XMFLOAT2);
	XMFLOAT2 GetPosition();
	
	void SetColor(XMFLOAT4);

	//Scale Access Methods
	void SetScale(float);
	float GetScale();

	//Calculate text size
	int GetTextWidth();
	int GetTextHeight();
	XMFLOAT2 GetTextDimensions();

	Font* GetFont();

private:
	//Types used for Rendering
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

	//Methods
	void Shutdown();
	//Set the Text Parameters for the shader - called before render
	bool SetParameters(ID3D11DeviceContext*);
	bool RenderText(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	//Variables
	shared_ptr<Font> m_pFont;

	string m_text;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_vertexCount, m_indexCount, m_length;
	XMFLOAT2 m_position;
	XMFLOAT2 m_local;
	XMFLOAT2 m_offset;

	XMFLOAT4 m_color;
	float m_fontScale;

};

#endif//_TEXT_H_