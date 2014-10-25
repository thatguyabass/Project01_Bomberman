//-----------------------------------------
//textclass.cpp
//-----------------------------------------
//Define the Text Class

#include "text.h"

Text::Text()
{
	m_pFont = 0;
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;

	m_fontScale = 1;
	m_offset = XMFLOAT2(0,0);
}

Text::Text(const Text& src)
{

}

Text::~Text()
{
	Shutdown();
}

bool Text::Initialize(ID3D11Device* device, shared_ptr<Font> font, string text,int maxLength, XMFLOAT2 position, XMFLOAT4 color, float fontScale)
{
	VertexType* pVertex;
	unsigned long* pIndex;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vData, iData;
	HRESULT result;

	//Store Font
	m_pFont = font;

	//Store the Font Scale
	m_fontScale = (fontScale + ASPECT.x + ASPECT.y) / 3;
	
	//Store Length 
	m_length = maxLength;

	//Store text
	SetText(text);

	//Store position
	m_local = position;
	m_position.x = position.x + m_offset.x;
	m_position.y = position.y + m_offset.y;

	//Store Color
	m_color = color;

	//Initialize the Sentence buffers
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;

	//Set the number of vertices in the vertex array
	m_vertexCount = 6 * maxLength;

	//Set the number of Indices in the index array
	m_indexCount = m_vertexCount;

	//Create the new vertex array
	pVertex = new VertexType[m_vertexCount];
	if(!pVertex)
		return false;

	//Create the new Index aray
	pIndex = new unsigned long[m_indexCount];
	if(!pIndex)
		return false;

	//Initialize the vertex array with zeros
	memset(pVertex, 0, sizeof(VertexType) * m_vertexCount);

	for(int c = 0; c < m_indexCount; c++)
		pIndex[c] = c;

	//Set up the description of the dynamic vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource structure pointer to the veretx data
	vData.pSysMem = pVertex;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	//Create the Vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vData, &m_pVertexBuffer);
	if(FAILED(result))
		return false;

	//Set up the index description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Subresource structure pointer to the index data
	iData.pSysMem = pIndex;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	//Create the Index Buffer
	result = device->CreateBuffer(&indexBufferDesc, &iData, &m_pIndexBuffer);
	if(FAILED(result))
		return false;

	//Release the Vertex 
	delete [] pVertex;
	pVertex = 0;

	//Release the Index
	delete [] pIndex;
	pIndex = 0;

	return true;
}

void Text::Update()
{
	XMFLOAT2 pos;
	pos.x = m_local.x + m_offset.x;
	pos.y = m_local.y - m_offset.y;

	this->SetPosition(pos);
}

//Move the text using speed and a direction
void Text::Move(float d, XMFLOAT2 dir)
{
	XMVECTOR t = XMVector2Normalize(XMLoadFloat2(&dir));

	float x = XMVectorGetX(t);
	float y = XMVectorGetY(t);

	m_position.x += (x * d);
	m_position.y += (y * d);
}

//Adjust the position using the offset provided
void Text::AdjustPos(XMFLOAT2 offset)
{
	m_position.x += offset.x;
	m_position.y += offset.y;
}

void Text::Change(string text, XMFLOAT2 position, XMFLOAT4 color, float fontScale)
{
	if(text.size() < m_length)
		m_text = text;
	
	m_position = position;
	m_color = color;
	m_fontScale = fontScale;
}

void Text::SetText(string text)
{
	if(text.size() < m_length)
		m_text = text;
}
string Text::GetText()
{
	return m_text;
}

void Text::SetColor(XMFLOAT4 color)
{
	m_color = color;
}

void Text::SetLocal(XMFLOAT2 local)
{
	m_local = local;
}
XMFLOAT2 Text::GetLocal()
{
	return m_local;
}

void Text::SetPosition(XMFLOAT2 position)
{
	m_position = position;
}
XMFLOAT2 Text::GetPosition()
{
	return m_position;
}

void Text::SetScale(float scale)
{
	m_fontScale = scale;
}
float Text::GetScale()
{
	return m_fontScale;
}

bool Text::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = SetParameters(context);
	if(!result)
		return false;

	RenderText(context, shader, world, view, ortho);

	return true;
}

bool Text::SetParameters(ID3D11DeviceContext* context)
{
	int numLetters;
	VertexType* pVertex;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* vData;

	//Store the length
	numLetters = m_text.size();

	//Avoid Overloading the buffer
	if(numLetters > m_length)
		return false;

	//Create the vertex array
	pVertex = new VertexType[m_vertexCount];
	if(!pVertex)
		return false;

	//Initialize the vertex array with zeros
	memset(pVertex, 0, sizeof(VertexType) * m_vertexCount);

	//Calculate the X and Y pixel location on the screen
	drawX = m_position.x;
	drawY = m_position.y;

	//build the vertex array 
	m_pFont->BuildVertexArray((void*)pVertex, m_text, drawX, drawY, m_color, m_fontScale); 

	//Lock the vertex buffer
	result = context->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	vData = (VertexType*)mappedResource.pData;

	//copy the data into the vertex buffer
	memcpy(vData, (void*)pVertex, sizeof(VertexType) * m_vertexCount);

	//Unlock the buffer
	context->Unmap(m_pVertexBuffer, 0);

	//Release the vertex array
	delete [] pVertex;
	pVertex = 0;

	return true;
}

bool Text::RenderText(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	unsigned int stride, offset;
	bool result;

	//Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler
	context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set the index buffer to active
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of Primitive
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the pixel color vector
	XMFLOAT4 color(m_color.x, m_color.y, m_color.z, 1.0f);

	//Render the text using the font shader
	result = shader->Render(context, m_indexCount, world, view, ortho, m_pFont->GetTexture());
	if(!result)
		return false;

	return true;
}

void Text::Shutdown()
{
	//Release the font
	if(m_pFont)
		m_pFont = 0;

	//Relase the vertex buffer
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

	//Release the index buffer
	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}
}

void Text::SetOffset(XMFLOAT2 offset)
{
	m_offset = offset;
}
XMFLOAT2 Text::GetOffset()
{
	return m_offset;
}

int Text::GetTextWidth()
{
	return m_pFont->GetTextDimension(m_text, m_fontScale).x;
}
int Text::GetTextHeight()
{
	return m_pFont->GetTextDimension(m_text, m_fontScale).y;
}
XMFLOAT2 Text::GetTextDimensions()
{
	return m_pFont->GetTextDimension(m_text, m_fontScale);
}

Font* Text::GetFont()
{
	return m_pFont.get();
}