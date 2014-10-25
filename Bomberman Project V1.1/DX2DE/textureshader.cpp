//-------------------------------------
//textureshader.cpp
//--------------------------------------
//Define the Texture Shader Class

#include "textureshader.h"

TextureShader::TextureShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
	m_pSampleState = 0;
}

TextureShader::TextureShader(const TextureShader& src)
{

}

TextureShader::~TextureShader()
{
	Shutdown();
}

bool TextureShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"../DX2DE/Shaders/texture.vs", L"../DX2DE/Shaders/texture.ps");
	if(!result)
		return false;

	return true;
}

void TextureShader::Shutdown()
{
	//Shutdown the vertex and pixel shaders and well as the related objects
	ShutdownShader();
}

bool TextureShader::Render(ID3D11DeviceContext* context, int index, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection, ID3D11ShaderResourceView* texture)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(context, world, view, projection, texture);
	if(!result)
		return false;

	RenderShader(context, index);

	return true;
}

bool TextureShader::InitializeShader(ID3D11Device* device, HWND hwnd, wstring vsFilename, wstring psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Initialize the pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Complie the Vertex Shader Code
	result = D3DX11CompileFromFile(vsFilename.c_str(), NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0 , NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		else
			MessageBox(hwnd, L"Failed to Load Vertex Shader",L"Vertex Shader Error", MB_OK);
		return false;
	}

	result = D3DX11CompileFromFile(psFilename.c_str(), NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0 , NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		else
			MessageBox(hwnd, L"Failed to load Pixel Shader", L"Pixel Shader Error", MB_OK);
		return false;
	}

	//Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if(FAILED(result))
		return false;

	//Create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if(FAILED(result))
		return false;

	//Create the vertex input layout description
	//this setup needs to match the VertexType structure in the ModelClass and int the shader
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

	polygonLayout[2].SemanticName = "COLOR";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//Get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pLayout);
	if(FAILED(result))
		return false;

	//Release the vertex Shader buffer
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	//Release the pixel shader buffer
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//Create the Content Buffer Pointer so we can access teh vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if(FAILED(result))
		return false;

	//Create a texture sampler state description
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

	//Create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_pSampleState);
	if(FAILED(result))
		return false;

	return true;
}

void TextureShader::ShutdownShader()
{
	//Release the Texture Sampler State
	if(m_pSampleState)
	{
		m_pSampleState->Release();
		m_pSampleState = 0;
	}

	//Release the Constant Matrix Buffer
	if(m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}

	//Release the Input Layout
	if(m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	//Releease the Vertex Shader
	if(m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}

	//Release the pixel shader
	if(m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}
}

void TextureShader::OutputShaderErrorMessage(ID3D10Blob* error, HWND hwnd, wstring shaderFilename)
{
	char* compileError;
	unsigned long bufferSize;
	ofstream fout;

	//Get a pointer to the error message text buffer
	compileError = (char*)(error->GetBufferPointer());

	//Get the length of the message
	bufferSize = error->GetBufferSize();
	
	//Open a file to Write the error message to
	fout.open("Shader-Error-Message.txt");

	//Write out the error message
	for(int c = 0; c < bufferSize; c++)
		fout << compileError[c];

	fout.close();

	//Relase the Error Message
	error->Release();
	error = 0;

	//Pop a Message up on the screen to notify the user to check teh text file for more information about the error
	MessageBox(hwnd, L"An Error has occured. See text file for more information", shaderFilename.c_str(), MB_OK);
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* context, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* mData;
	unsigned int bufferNumber;

	//Transpose the matrices to prepare them for the shader
	XMMATRIX _world = XMMatrixTranspose(XMLoadFloat4x4(&world));
	XMMATRIX _view = XMMatrixTranspose(XMLoadFloat4x4(&view));
	XMMATRIX _projection = XMMatrixTranspose(XMLoadFloat4x4(&projection));

	//Lock the constant buffer so it can be writtern to
	result = context->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	//Get a pointer to the mData in the constant buffer
	mData = (MatrixBufferType*)mappedResource.pData;

	//Copy the matraces into the constant buffer
	XMStoreFloat4x4(&mData->world, _world);
	XMStoreFloat4x4(&mData->view, _view);
	XMStoreFloat4x4(&mData->projection, _projection);

	//Unlock the constant buffer
	context->Unmap(m_pMatrixBuffer, 0);

	//Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Now set the constant buffer in the vertex shader with the updated values
	context->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	//Set the shader texture resource in the pixel shader
	context->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* context, int index)
{
	//Set the vertex ionput layout
	context->IASetInputLayout(m_pLayout);

	//Set the vertex and pixel shaders that will be used to render this object
	context->VSSetShader(m_pVertexShader, NULL, 0);
	context->PSSetShader(m_pPixelShader, NULL, 0);

	//Set the sampler state in the pixel shader
	context->PSSetSamplers(0, 1, &m_pSampleState);

	//Render the object
	context->DrawIndexed(index, 0, 0);
}