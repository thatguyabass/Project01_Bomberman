//--------------------------------------------------------------------------
//shaderManager.h
//--------------------------------------------------------------------------
//This class will act as the base for every shader class.

#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include<D3D11.h>
#include<D3DX11async.h>

#include<Windows.h>
#include<xnamath.h>
#include<fstream>

using namespace std;

class BaseShader
{
public:
	BaseShader();
	BaseShader(const BaseShader&);
	~BaseShader();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&);//, ID3D11ShaderResourceView*);

protected:
	//Buffer
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	//Methods
	void Shutdown();
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&);//, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	//Variables
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	//ID3D11SamplerState* m_pSampleState;

};

#endif//_SHADER_MANAGER_H_
