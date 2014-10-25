//-------------------------------------------------
//colorshader.cpp
//-------------------------------------------------
//Define the Color Shader Class object

#include "colorshader.h"

ColorShader::ColorShader()
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pMatrixBuffer = 0;
}

ColorShader::ColorShader(const ColorShader& src)
{

}

ColorShader::~ColorShader()
{

}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shader
	result = InitializeShader(device, hwnd, L"../DX2DE/Shaders/color.vs", L"../DX2DE/Shaders/color.ps");
	if(!result)
		return false;

	return true;
}

void ColorShader::Shutdown()
{
	//Shutdown the pixel and vertex shaders
	ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext* context, int index, XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(context, world, view, projection);
	if(!result)
		return false;

	//Now render the prepared buffers with the shader
	RenderShader(context, index);

	return true;
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	
	//------------------------------------------------------

	//Compile the vertex shader code
	result = D3DX11CompileFromFile(vsFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0 , NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		//if the shader failed to compile it should be writen somthing to the new error message
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		//if the error message is NULL then it couldnt find the shader file itself
		else
			MessageBox(hwnd, vsFileName, L"Missing Shader File", MB_OK);
		
		return false;
	}

	//Compile the pixel shader code
	result = D3DX11CompileFromFile(psFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psFileName);
		else
			MessageBox(hwnd, psFileName, L"Missing Shader File", MB_OK);
		return false;
	}
	
	//Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if(FAILED(result))
		return false;

	//Create the pixel shader from teh buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if(FAILED(result))
		return false;

	//------------------------------------------------------

	//NOw setup the layout of the data that goes into the shader
	//this setup needs to match the VetexType structure in ModelClass and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	//Release the vertex and pixel shader buffers since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//------------------------------------------------------

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if(FAILED(result))
		return false;

	return true;
}

void ColorShader::ShutdownShader()
{
	//Release the matrix constant buffer
	if(m_pMatrixBuffer)
	{
		m_pMatrixBuffer->Release();
		m_pMatrixBuffer = 0;
	}

	//Release Layout
	if(m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	//Release vertex shader
	if(m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}

	//Release pixel shader
	if(m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, WCHAR* shaderFileName)
{
	char* compileError;
	unsigned long bufferSize;
	unsigned long c;
	ofstream fout;

	//Get a pointer to the error message text buffer
	compileError = (char*)(blob->GetBufferPointer());

	//Get the length of the message
	bufferSize = blob->GetBufferSize();

	//Open a file to write the error message to
	fout.open("Shader-Error.txt");

	//Write out the error message
	for(c = 0; c < bufferSize; c++)
	{
		fout << compileError[c];
	}

	//Close File
	fout.close();

	//Release the error message
	blob->Release();
	blob = 0;

	//Pop a message up on the screen to notify the user to check the text file for compile errors
	MessageBox(hwnd, L"Error compiling shader.", shaderFileName, MB_OK);
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* context, XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataType;
	unsigned int bufferNumber;

	//Transpose the matrices to prepare them for the shader
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	//Lock the constant buffer so it can be written to
	result = context->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	//Get a pointer to the data in the constant buffer
	dataType = (MatrixBufferType*)mappedResource.pData;

	//Copy the matrices into the constant buffer
	dataType->world = world;
	dataType->view = view;
	dataType->projection = projection;

	//Unlock the constant buffer
	context->Unmap(m_pMatrixBuffer, 0);
	
	//Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Finaly set the constant buffer in the vertex shader with teh updated values
	context->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* context, int index)
{
	//Set the vertex input layout
	context->IASetInputLayout(m_pLayout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	context->VSSetShader(m_pVertexShader, NULL, 0);
	context->PSSetShader(m_pPixelShader, NULL, 0);

	//Render the object
	context->DrawIndexed(index, 0, 0);
}