//----------------------------------
//d3d.h
//----------------------------------
//base object directX graphics. Handles Direct X 3D

#ifndef _DIRECTX_11_3D_H_
#define _DIRECTX_11_3D_H_

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include<Windows.h>
#define _XM_NO_INTRINSICS_
#include<xnamath.h>

#include<DXGI.h>
#include<D3Dcommon.h>
#include<D3D11.h>
#include<D3D10.h>

#include "Global.h"

class D3D
{
public:
	D3D();
	D3D(const D3D&);
	~D3D();

	bool Initialize(bool vsync, bool fullScreen, float depth, float near);

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMFLOAT4X4&);
	void GetWorldMatrix(XMFLOAT4X4&);
	void GetOrthoMatrix(XMFLOAT4X4&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void GetVideoCardInfo(char*, int&);

private:
	//Methods
	void Shutdown();
	void OutputD3DErrorMessage(HWND);

	//Variables
	bool m_vsyncEnabled;
	int m_videoCardMemory;
	char m_videoCardDescription[126];
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilState* m_pDepthDisabledStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;
	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT4X4 m_orthoMatrix;

	ID3D11BlendState* m_pAlphaEnabledBlendState;
	ID3D11BlendState* m_pAlphaDisabledBlendState;

};

#endif //_DIRECTX_11_3D_H_
