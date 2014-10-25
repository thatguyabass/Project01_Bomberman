//----------------------------------------
//colorshader.h
//----------------------------------------
//Color Shader Class is what will invoke HLSL shaders for drawuing 3D models

#ifndef _COLOR_SHADER_H_
#define _COLOR_SHADER_H_

#include<D3D11.h>
#include<D3DX11async.h>

#include<Windows.h>
#include<xnamath.h>
#include<fstream>

using namespace std;

class ColorShader
{
public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&);

private:
	//Matrix Buffer Type
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	//Methods
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&);
	void RenderShader(ID3D11DeviceContext*, int);

	//Variables
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;

};

#endif //_COLOR_SHADER_H_