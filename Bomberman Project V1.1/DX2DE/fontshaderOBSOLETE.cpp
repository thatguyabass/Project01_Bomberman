//----------------------------------------------------------
//fontshader.cpp
//----------------------------------------------------------
//Define the font shader class

#include "fontshaderOBSOLETE.h"

FontShader::FontShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pConstantBuffer = 0;
	m_pPixelBuffer = 0;
	m_pSamplerState = 0;
}

FontShader::FontShader(const FontShader& src)
{

}

FontShader::~FontShader()
{
	Shutdown();
}

bool FontShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	
	//Initialize the vertex and pixel shader
	result = InitializeBuffer(device, hwnd, L"../DX2DE/Shaders/font.vs", L"../DX2DE/Shaders/font.ps");
	if(!result)
		return false;

	return true;
}

void FontShader::Shutdown()
{
	//Relase the shader
	ShutdownShader();
}

bool FontShader::Render(ID3D11DeviceContext* context, int index, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor)
{
	bool result;

	//Set the shader parameters
	result = SetShaderParameters(context, world, view, projection, texture, pixelColor);
	if(!result)
		return false;

	//Render the Object
	RenderShader(context, index);

	return true;
}

bool FontShader::InitializeBuffer(ID3D11Device* device, HWND hwnd, wstring vsFile, wstring psFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;
	
	//Initialize the Pointers
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Complie the VertexShader
	result = D3DX11CompileFromFile(vsFile.c_str(), NULL, NULL, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFile);
		else
			MessageBox(hwnd, vsFile.c_str(), L"Error With the Font Vertex Shader", MB_OK);
		return false;
	}

	result = D3DX11CompileFromFile(psFile.c_str(), NULL, NULL, "FontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psFile);
		else
			MessageBox(hwnd, psFile.c_str(), L"Error With the Font Pixel Shader", MB_OK);
		return false;
	}

	//Create the vertex shader
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if(FAILED(result))
		return false;

	//Create the pixel shader
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if(FAILED(result))
		return false;

	//Create the Input Layout
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//Get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pLayout);
	if(FAILED(result))
		return false;

	//Release the vertex Buffer
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	//Relase the pixel Buffer
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//Set up the constant buffer description
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	//Create the constant buffer
	result = device->CreateBuffer(&constantBufferDesc, NULL, &m_pConstantBuffer);
	if(FAILED(result))
		return false;

	//Create the texture sampler 
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the texture sampler
	result = device->CreateSamplerState(&samplerDesc, &m_pSamplerState);
	if(FAILED(result))
		return false;

	//Set up the pixel constant buffer description
	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;
	
	//Create the constant pixel buffer
	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pPixelBuffer);
	if(FAILED(result))
		return false;

	return true;
}

void FontShader::ShutdownShader()
{
	//Release the vertex shader
	if(m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}

	//Release the Pixel Shader
	if(m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}

	//Release the Input Layout
	if(m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	//Release the Constant Buffer
	if(m_pConstantBuffer)
	{
		m_pConstantBuffer->Release();
		m_pConstantBuffer = 0;
	}

	//Release the Pixel Buffer
	if(m_pPixelBuffer)
	{
		m_pPixelBuffer->Release();
		m_pPixelBuffer = 0;
	}

	//Release the Sampler State
	if(m_pSamplerState)
	{
		m_pSamplerState->Release();
		m_pSamplerState = 0;
	}
}

void FontShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, wstring file)
{
	char* compileError;
	unsigned long bufferSize;
	ofstream out;

	//Get a pointer to the error message
	compileError = (char*)(errorMessage->GetBufferPointer());

	//Get the message length
	bufferSize = errorMessage->GetBufferSize();

	//Open File
	out.open("Font-Shader-Error.txt");

	//Write out the message
	for(int c = 0; c < bufferSize; c++)
		out << compileError[c];

	//Close the file
	out.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, file.c_str(), L"An Error has occured in the font shader, See text file for information", MB_OK);
}

bool FontShader::SetShaderParameters(ID3D11DeviceContext* context, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection, ID3D11ShaderResourceView* texture, XMFLOAT4 color)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBufferType* cData;
	unsigned int bufferNumber;
	PixelBufferType* pData;	
	
	XMMATRIX _world = XMMatrixTranspose(XMLoadFloat4x4(&world));
	XMMATRIX _view = XMMatrixTranspose(XMLoadFloat4x4(&view));
	XMMATRIX _projection = XMMatrixTranspose(XMLoadFloat4x4(&projection));

	//Lock the Constant buffer
	result = context->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	//Get the Pointer to the data
	cData = (ConstantBufferType*)mappedResource.pData;

	XMStoreFloat4x4(&cData->world, _world);
	XMStoreFloat4x4(&cData->view, _view);
	XMStoreFloat4x4(&cData->projection, _projection);

	context->Unmap(m_pConstantBuffer, 0);

	//Set Buffer number
	bufferNumber = 0;
	
	//Update the vertex shader
	context->VSSetConstantBuffers(bufferNumber, 1, &m_pConstantBuffer);

	//Set the texture to the pixel shader
	context->PSSetShaderResources(0, 1, &texture);

	//Lock the pixel constant buffer
	result = context->Map(m_pPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	pData = (PixelBufferType*)mappedResource.pData;

	//Set the pixel color to the pixel shader
	pData->color = color;

	context->Unmap(m_pPixelBuffer, 0);

	//Set Buffer Number
	bufferNumber = 0;

	//Update the Pixel Constant Buffer
	context->PSSetConstantBuffers(bufferNumber, 1, &m_pPixelBuffer);
	
	return true;
}

void FontShader::RenderShader(ID3D11DeviceContext* context, int index)
{
	//Set the vertex input layout
	context->IASetInputLayout(m_pLayout);

	//Set the vertex and pixel shaders that will be used to render
	context->VSSetShader(m_pVertexShader, NULL, 0);
	context->PSSetShader(m_pPixelShader, NULL, 0);

	//Set the sampler state
	context->PSSetSamplers(0, 1, &m_pSamplerState);

	//Render
	context->DrawIndexed(index, 0, 0);
}