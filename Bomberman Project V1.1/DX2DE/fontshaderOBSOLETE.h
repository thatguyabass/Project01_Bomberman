//-------------------------------------------------
//fontshader.h
//-------------------------------------------------
//Handles rendering the font using the font vetex and pixel shader

#ifndef _FONT_SHADER_H_
#define _FONT_SHADER_H_

#include<D3D11.h>
#include<D3DX11async.h>

#include<Windows.h>
#include<xnamath.h>
#include<fstream>

using namespace std;

class FontShader
{
public:
	FontShader();
	FontShader(const FontShader&);
	~FontShader();

	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	//Types
	struct ConstantBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	struct PixelBufferType
	{
		XMFLOAT4 color;
	};

	//Methods
	void Shutdown();
	bool InitializeBuffer(ID3D11Device*, HWND, wstring, wstring);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, wstring);

	bool SetShaderParameters(ID3D11DeviceContext*, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&, ID3D11ShaderResourceView*, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

	//Variables
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pPixelBuffer;
	ID3D11SamplerState* m_pSamplerState;

};

#endif//_FONT_SHADER_H_