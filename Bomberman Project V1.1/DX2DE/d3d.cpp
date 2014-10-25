//----------------------------------------
//d3d.cpp
//----------------------------------------
//Define the D3D class

#include "d3d.h"

D3D::D3D()
{
	m_pSwapChain = 0;
	m_pDevice = 0;
	m_pDeviceContext = 0;
	m_pRenderTargetView = 0;
	m_pDepthStencilBuffer = 0;
	m_pDepthStencilState = 0;
	m_pDepthDisabledStencilState = 0;
	m_pDepthStencilView = 0;
	m_pRasterState = 0;
	m_pAlphaEnabledBlendState = 0;
	m_pAlphaDisabledBlendState = 0;
}

D3D::D3D(const D3D& src)
{

}

D3D::~D3D()
{
	Shutdown();
}

bool D3D::Initialize(bool vsync, bool full, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, c, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDesc;

	int width = Global::GetScreenWidth();
	int height = Global::GetScreenHeight();
	HWND hwnd = Global::GetHWND();
	
	//Get Video card description and monitor description
	//--------------------------------------------------------------------------------
	
	//Store the vsync
	m_vsyncEnabled = vsync;

	//Create a DirectX graphics interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&factory);
	if(FAILED(result))
		return false;

	//Use the factory to create an adapter for the primary graphics interface (video card)
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
		return false;

	//Enumerate the primary adapter output(monitor)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
		return false;

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor)
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
		return false;

	//Create a list to hold all possable display modes for this monitor/video card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	//Now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
		return false;

	//Now go through all the display modes and find the one that matches the screen width and height.
	//when a match is found store the numerator and denominator of the refresh rate for that monitor
	for(c = 0; c < numModes; c++)
	{
		if(displayModeList[c].Width == (unsigned int)width)
		{
			if(displayModeList[c].Height == (unsigned int)height)
			{
				numerator = displayModeList[c].RefreshRate.Numerator;
				denominator = displayModeList[c].RefreshRate.Denominator;
			}
		}
	}

	//Video Card Description
	//--------------------------------------------------------------------------------
	
	//Get the adapter(video card) description
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
		return false;

	//Store the dedicated video card memory in megabytes
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//convert the name of the video card to a character array and store it
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
		return false;
	
	//Clean up variables that will no longer be needed
	//--------------------------------------------------------------------------------

	//Release the display mode list
	delete [] displayModeList;
	displayModeList = 0;

	//Release the adaptor output
	adapterOutput->Release();
	adapterOutput = 0;

	//release the adapter
	adapter->Release();
	adapter = 0;

	//release the factory
	factory->Release();
	factory = 0;
	
	//Initialize the swap chain description and buffer settings
	//--------------------------------------------------------------------------------
	
	//Initialize the swap chain description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//Set to a single back buffer
	swapChainDesc.BufferCount = 1;

	//Set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	//Seet regular 32-bit surface for teh back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		
	//Apply the swap chains description settings
	//--------------------------------------------------------------------------------
	
	//Set the Refreash rate of the back buffer
	if(!m_vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//Set the handle for the window to render to
	swapChainDesc.OutputWindow = hwnd;

	//Turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//Set to full screen or windowed mode
	if(full)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	//set the scan line ordering the scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Dont set the advanced flags
	swapChainDesc.Flags = 0;

	//Set Feature level to DirectX 11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//Create the Swap Chain using the swap chains description
	//--------------------------------------------------------------------------------

	//Create the swap chain, Direct3D device, and Direct3D device context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
	if(FAILED(result))
		return false;
	
	//Initialize the back buffer
	//--------------------------------------------------------------------------------

	//Get the pointer to the back buffer
	result = m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
		return false;

	//Create the render target view with back buffer pointer
	result = m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView);
	if(FAILED(result))
		return false;

	//Release pointer to the back buffer as we no longer need it
	backBufferPtr->Release();
	backBufferPtr = 0;

	//Settings up the description of the depth buffer
	//--------------------------------------------------------------------------------

	//Initialize the description of the depth buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//Set up the description of the depth buffer
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Create the texture for the depth buffer using the filled out description
	result = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if(FAILED(result))
		return false;
	
	//Initialze the description of the stencil state
	//--------------------------------------------------------------------------------

	//Initialize the Description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//Set up the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the depth stencil state
	result = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if(FAILED(result))
		return false;

	//Set the depth stencil state
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState,1);
	
	//Initialize the depth stencil view
	//--------------------------------------------------------------------------------

	//Initialize the depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	
	//Set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the Depth stencil view
	result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if(FAILED(result))
		return false;

	//Bind the render target view and depth stencil buffer to the output render pipeline
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		
	//Initialize the raster description
	//--------------------------------------------------------------------------------

	//Setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//Create the rasterizer state from the description we just filled out
	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	if(FAILED(result))
		return false;

	//Now set the Rasterizer state
	m_pDeviceContext->RSSetState(m_pRasterState);
		
	//View Port Rendering setup
	//--------------------------------------------------------------------------------

	//Setup the viewport for rendering
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//Create the viewport
	m_pDeviceContext->RSSetViewports(1, &viewport);
	
	//Matrixs
	//--------------------------------------------------------------------------------

	//Setup the projection matrix
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)width / (float)height;

	XMMATRIX p;
	XMMATRIX w;
	XMMATRIX o;

	//Create the Projection Matrix
	p = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	
	//Initialize the world matrix to the identity matrix
	w = XMMatrixTranslation(-1,-10,0);//XMMatrixIdentity();

	//Create the orthographic projection matrix for 2D rendering
	o = XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth);

	XMStoreFloat4x4(&m_projectionMatrix, p);
	XMStoreFloat4x4(&m_worldMatrix, w);
	XMStoreFloat4x4(&m_orthoMatrix, o);

	//The only difference between the two Depth Stencil states is that the Dissable Depth is turn off in this one
	//Clear the Second Depth Stencil State
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	//Now Create the second depth stencil state, The only difference is the DepthEnabled is set to false
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the new Depth stencil
	result = m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState);
	if(FAILED(result))
		return false;

	//Clear the blend state description
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	//Create the alpha enabled description
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//Create the enabled blend state
	result = m_pDevice->CreateBlendState(&blendStateDesc, &m_pAlphaEnabledBlendState);
	if(FAILED(result))
		return false;

	//Modify the Blend Enabled 
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	//Create the disabled blend state
	result = m_pDevice->CreateBlendState(&blendStateDesc, &m_pAlphaDisabledBlendState);
	if(FAILED(result))
		return false;

	return true;
}

void D3D::Shutdown()
{
	//Before the shutting down set the windowed mode  to true or when you release the swap chain it will throw an exception
	if(m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, NULL);

	//Relase the Enabled Blend State
	if(m_pAlphaEnabledBlendState)
	{
		m_pAlphaEnabledBlendState->Release();
		m_pAlphaEnabledBlendState = 0;
	}

	//Relase the Disabled Blend State
	if(m_pAlphaDisabledBlendState)
	{
		m_pAlphaDisabledBlendState->Release();
		m_pAlphaDisabledBlendState = 0;
	}

	//Release the Raster State
	if(m_pRasterState)
	{
		m_pRasterState->Release();
		m_pRasterState = 0;
	}
	//Release the Depth Stencil Buffer
	if(m_pDepthStencilBuffer)
	{
		m_pDepthStencilBuffer->Release();
		m_pDepthStencilBuffer = 0;
	}
	//Release the Depth Stencil State
	if(m_pDepthStencilState)
	{
		m_pDepthStencilState->Release();
		m_pDepthStencilState = 0;
	}
	//Release the Depth Disabled Stencil State
	if(m_pDepthDisabledStencilState)
	{
		m_pDepthDisabledStencilState->Release();
		m_pDepthDisabledStencilState = 0;
	}
	//Release the Depth Stencil View
	if(m_pDepthStencilView)
	{
		m_pDepthStencilView->Release();
		m_pDepthStencilView = 0;
	}
	//Release the Render Target View
	if(m_pRenderTargetView)
	{
		m_pRenderTargetView->Release();
		m_pRenderTargetView = 0;
	}
	//Release the Device Context
	if(m_pDeviceContext)
	{
		m_pDeviceContext->Release();
		m_pDeviceContext = 0;
	}
	//Release the Device
	if(m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = 0;
	}
	//Release the Swap Chain
	if(m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = 0;
	}
}

void D3D::BeginScene(float r, float g, float b, float a)
{
	float color[4];

	//Setup the color to clear the buffer to
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	//Clear the back buffer
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	//Clear the depth buffer
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void D3D::EndScene()
{
	//Present the back buffer to the screen since rendering is complete
	if(m_vsyncEnabled)
	{
		//Lock the screen refresh rate
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		//Present as fast as possable
		m_pSwapChain->Present(0, 0);
	}
}

ID3D11Device* D3D::GetDevice()
{
	return m_pDevice;
}
ID3D11DeviceContext* D3D::GetDeviceContext()
{
	return m_pDeviceContext;
}

void D3D::GetProjectionMatrix(XMFLOAT4X4& matrix)
{
	matrix = m_projectionMatrix;
}
void D3D::GetWorldMatrix(XMFLOAT4X4& matrix)
{
	matrix = m_worldMatrix;
}
void D3D::GetOrthoMatrix(XMFLOAT4X4& matrix)
{
	matrix = m_orthoMatrix;
}

void D3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}

void D3D::TurnZBufferOn()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}
void D3D::TurnZBufferOff()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
}

void D3D::TurnOnAlphaBlending()
{
	float blend[4];
	
	blend[0] = 0.0f;
	blend[1] = 0.0f;
	blend[2] = 0.0f;
	blend[3] = 0.0f;

	//Trun on the alpha blending
	m_pDeviceContext->OMSetBlendState(m_pAlphaEnabledBlendState, blend, 0xffffffff);
}

void D3D::TurnOffAlphaBlending()
{
	float blend[4];

	blend[0] = 0.0f;
	blend[1] = 0.0f;
	blend[2] = 0.0f;
	blend[3] = 0.0f;

	//Trun on the alpha blending
	m_pDeviceContext->OMSetBlendState(m_pAlphaDisabledBlendState, blend, 0xffffffff);
}