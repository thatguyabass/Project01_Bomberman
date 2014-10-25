//-------------------------------------------------
//textureshaderclass.h
//-------------------------------------------------
//this class will draw 3d objects using vertex and pixel shaders

#ifndef _TEXTURE_SHADER_H_
#define _TEXTURE_SHADER_H_

#include<D3D11.h>
#include<D3DX11async.h>

#include<Windows.h>
#include<xnamath.h>
#include<fstream>

using namespace std;

class TextureShader
{
public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&, ID3D11ShaderResourceView*);

private:
	//Buffer
	struct MatrixBufferType
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

	//Methods
	void Shutdown();
	bool InitializeShader(ID3D11Device*, HWND, wstring, wstring);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, wstring);

	bool SetShaderParameters(ID3D11DeviceContext*, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	//Variables
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11SamplerState* m_pSampleState;

};

#endif //_TEXTURE_SHADER_H_