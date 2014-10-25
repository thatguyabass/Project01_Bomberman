//--------------------------------------------------
//base2D.cpp
//--------------------------------------------------
//Define the Base2D class

#include "base2D.h"

Base2D::Base2D()
{
	//Float the pointers
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pTexture = 0;

	m_boundsMod = 1.0f;
	m_pBox = 0;

	m_colorOverride = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_offset = XMFLOAT2(0,0);
}

Base2D::Base2D(const Base2D& src)
{

}

Base2D::~Base2D()
{
	Shutdown();
}

bool Base2D::Initialize(ID3D11Device* device, int imageWidth, int imageHeight, XMFLOAT2 position, float rot)
{
	bool result;

	//Store the Image Size
	m_imageWidth = imageWidth;
	m_imageHeight = imageHeight;

	//Initalize the previous rendering positions and rotation to negitive one
	//This will reset the value forcing the dynamic buffer to render it again saving cycles
	m_previousPosX = -1;
	m_previousPosY = -1;
	m_previousRot = -1;

	//Initalize the vertex and index buffers
	result = InitializeBuffer(device);
	if(!result)
		return false;

	//Initialize the Objects Position, rotaiton and scale
	InitializeTransform(position, rot);
	
	return true;
}

void Base2D::Shutdown()
{
	//Release the Texture
	ReleaseTexture();

	//Signel the shut down for each class
	ShutdownBuffer();

	if(m_pBox)
	{
		m_pBox.reset();
		m_pBox = 0;
	}
}

//Base Update will only update the position offset using Local + offset
void Base2D::Update(float gameTime)
{
	XMFLOAT2 pos;
	pos.x = LOCAL.x + m_offset.x;
	pos.y = LOCAL.y - m_offset.y;

	this->SetPosition(pos);
}

bool Base2D::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	//Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen
	result = UpdateBuffer(context);
	if(!result)
		return false;

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffer(context);

	//Render the Object using the shader
	result = RenderShader(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

bool Base2D::InitializeBuffer(ID3D11Device* device)
{
	VertexType* pVertex;
	unsigned long* pIndex;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vData;
	D3D11_SUBRESOURCE_DATA iData;
	HRESULT result;
	
	//Set the vertex number for the vertex array
	m_vertexCount = 6;	//Value is 6 to form a square

	//Set the index number for the index array
	m_indexCount = m_vertexCount;

	pVertex = new VertexType[m_vertexCount];
	if(!pVertex)
		return false;

	pIndex = new unsigned long[m_indexCount];
	if(!pIndex)
		return false;

	//Initialize the vertex array with zeros
	memset(pVertex, 0, (sizeof(VertexType) * m_vertexCount));

	//Load the index array with data
	for(int c = 0; c < m_indexCount; c++)
		pIndex[c] = c;

	//Setup the description of the dynamic vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Get the subresource structure
	vData.pSysMem = pVertex;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	//Now Create the vertex Buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vData, &m_pVertexBuffer);
	if(FAILED(result))
		return false;

	//Setup the description of the 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Get the Subresource structure
	iData.pSysMem = pIndex;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &iData, &m_pIndexBuffer);
	if(FAILED(result))
		return false;

	//Release the Vertex Array
	delete [] pVertex;
	pVertex = 0;

	//Release the Index Array
	delete [] pIndex;
	pIndex = 0;

	return true;
}

void Base2D::ShutdownBuffer()
{
	//Release the Vertex Buffer
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

	//Release the Index Buffer
	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}
}

bool Base2D::UpdateBuffer(ID3D11DeviceContext* context)
{
	float left, right, top, bottom;
	VertexType* pVertex;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* pVIndex;
	HRESULT result;

	//Cancel Update if Position, Rotation and Color have all remained the same since last update
	if(POSITION.x == m_previousPosX && POSITION.y == m_previousPosY && ROTATION == m_previousRot && XMVector4Equal(XMLoadFloat4(&m_colorOverride), XMLoadFloat4(&m_previousColor)))
		return true;

	//Load the changed position
	m_previousPosX = POSITION.x;
	m_previousPosY = POSITION.y;
	m_previousRot = ROTATION;

	//The four sides of the image need to be processed
	//Calculate the Left Side
	left = POSITION.x - (m_imageWidth / 2);
	right = left + m_imageWidth;

	//Calculate the Top Side
	top = POSITION.y + (m_imageHeight / 2);
	bottom = top - m_imageHeight;

	pVertex = new VertexType[m_vertexCount];
	if(!pVertex)
		return false;

	//Update the vertex positions
	VertexUpdate(pVertex, left, right, top, bottom);

	//Lock the vertex buffer so it can be written to
	result = context->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	//Get the pointer to the vertex buffer
	pVIndex = (VertexType*)mappedResource.pData;

	//Copy the data into the vertex buffer
	memcpy(pVIndex, (void*)pVertex, (sizeof(VertexType) * m_vertexCount));

	//Unlock the vertex buffer
	context->Unmap(m_pVertexBuffer, 0);

	//Release the vertex type
	delete [] pVertex;
	pVertex = 0;

	return true;
}
void Base2D::RenderBuffer(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	//Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//Set the Vertex Buffer to active so it can be rendered
	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set the Index Buffer to active so it can be rendered
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Base2D::RenderShader(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& projection)
{
	bool result;

	result = shader->Render(context, this->GetIndexCount(), world, view, projection, this->GetTexture());
	if(!result)
		return false;

	return true;
}

bool Base2D::LoadTexture(ID3D11Device* device, wstring texture)
{
	bool result;

	//Create a new Texture
	m_pTexture = shared_ptr<Texture>(new Texture);
	if(!m_pTexture)
		return false;
	
	//Load the Texture
	result = m_pTexture->Initialize(device, texture);
	if(!result)
		return false;

	return true;
}

void Base2D::ReleaseTexture()
{
	if(m_pTexture)
	{
		m_pTexture.reset();
		m_pTexture = 0;
	}
}

int Base2D::GetIndexCount()
{
	return m_indexCount;
}
ID3D11ShaderResourceView* Base2D::GetTexture()
{
	return m_pTexture->GetTexture();
}

void Base2D::InitializeTransform(XMFLOAT2 pos, float rot)
{
	LOCAL = XMFLOAT2(pos.x, pos.y);
	POSITION = XMFLOAT2(pos.x + m_offset.x, pos.y - m_offset.y);
	ROTATION = XMConvertToRadians(rot);
}

void Base2D::VertexUpdate(VertexType* pVertex, float left, float right, float top, float bottom)
{
	if(m_pBox != NULL)
		m_pBox->Update(POSITION);

	float S = sin(ROTATION);
	float C = cos(ROTATION);

	float OL = (m_imageWidth / 2) * -1;
	float OR = (m_imageWidth /2);
	float OT = (m_imageHeight / 2);
	float OB = (m_imageHeight / 2) * -1;

	float LTX = (OL * C) - (OT * S);  
	float LTY = (OL * S) + (OT * C);

	float RTX = (OR * C) - (OT * S);
	float RTY = (OR * S) + (OT * C);

	float LBX = (OL * C) - (OB * S);
	float LBY = (OL * S) + (OB * C);

	float RBX = (OR * C) - (OB * S);
	float RBY = (OR * S) + (OB * C);

	OL = left - OL;
	OR = right - OR;
	OT = top - OT;
	OB = bottom - OB;

	//Load the vertex Array with data
	pVertex[0].position = XMFLOAT3(LTX + OL, LTY + OT, 0);
	pVertex[1].position = XMFLOAT3(RBX + OR, RBY + OB, 0);
	pVertex[2].position = XMFLOAT3(LBX + OL, LBY + OB, 0);
	pVertex[3].position = XMFLOAT3(LTX + OL, LTY + OT, 0);
	pVertex[4].position = XMFLOAT3(RTX + OR, RTY + OT, 0);
	pVertex[5].position = XMFLOAT3(RBX + OR, RBY + OB, 0);

	//Update the Texture UV
	UpdateTexture(pVertex);
	
	//Update the Textures Color
	UpdateColor(pVertex);
}

void Base2D::UpdateTexture(VertexType* pVertex)
{
	//Default UV Map
	pVertex[0].texture = XMFLOAT2(0.0f, 0.0f);
	pVertex[1].texture = XMFLOAT2(1.0f, 1.0f);
	pVertex[2].texture = XMFLOAT2(0.0f, 1.0f);
	pVertex[3].texture = XMFLOAT2(0.0f, 0.0f);
	pVertex[4].texture = XMFLOAT2(1.0f, 0.0f);
	pVertex[5].texture = XMFLOAT2(1.0f, 1.0f);
}

void Base2D::UpdateColor(VertexType* pVertex)
{
	//Update Texture Color
	pVertex[0].color = m_colorOverride;
	pVertex[1].color = m_colorOverride;
	pVertex[2].color = m_colorOverride;
	pVertex[3].color = m_colorOverride;
	pVertex[4].color = m_colorOverride;
	pVertex[5].color = m_colorOverride;
}

//Check if a collision has occurred and return the side of the collision
bool Base2D::CheckIntersect(BoundingBox* box2, int& side)
{
	if(m_pBox && box2)
		return m_pBox->BoundingBoxIntersect(m_pBox.get(), box2, side);
	else
		return false;
}

bool Base2D::CheckIntersect(BoundingBox* box2)
{
	if(m_pBox && box2)
		return m_pBox->BoundingBoxIntersect(m_pBox.get(), box2);
	else
		return false;
}

//Create and Position the Bounding Box
void Base2D::SetBox()
{
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox());
	m_pBox->Set(m_imageWidth, m_imageHeight, POSITION);
}
BoundingBox* Base2D::GetBox()
{
	return m_pBox.get();
}

XMFLOAT2 Base2D::GetPosition()
{
	return POSITION;
}
void Base2D::SetPosition(XMFLOAT2 pos)
{
	POSITION = pos;
}

XMFLOAT2 Base2D::GetLocal()
{
	return LOCAL;
}
void Base2D::SetLocal(XMFLOAT2 local)
{
	LOCAL = local;
}

void Base2D::SetColor(XMFLOAT4 color)
{
	m_colorOverride = color;
}
XMFLOAT4 Base2D::GetColor()
{
	return m_colorOverride;
}

void Base2D::SetDimensions(XMFLOAT2 size)
{
	m_imageWidth = size.x;
	m_imageHeight = size.y;
}
XMFLOAT2 Base2D::GetDimensions()
{
	return XMFLOAT2(m_imageWidth, m_imageHeight);
}

XMFLOAT2 Base2D::GetOffset()
{
	return m_offset;
}
void Base2D::SetOffset(XMFLOAT2 off)
{
	m_offset = off;
}