//--------------------------------------------------------------------------------
//Cursor.cpp
//--------------------------------------------------------------------------------
//Define the Cursor Class

#include "Cursor.h"

Cursor::Cursor()
{
	m_offset = XMFLOAT2(0,0);
}

Cursor::~Cursor()
{
	Shutdown();
}

bool Cursor::Initialize(ID3D11Device* device, wstring texture, int imageWidth, int imageHeight)
{
	bool result;

	result = Base2D::Initialize(device, imageWidth, imageHeight, XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	result = Base2D::LoadTexture(device, texture);
	if(!result)
		return false;

	//Set the bounding box to be used with the GUI elements
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(m_imageWidth, m_imageHeight, POSITION);

	return true;
}

void Cursor::Update(float gameTime)
{
	//Position the cursor to follow the Mouse Position
	XMFLOAT2 pos;
	XMFLOAT2 mPos = Input::GetMousePosition();
	pos.x = mPos.x + m_offset.x;
	pos.y = mPos.y - m_offset.y;

	this->SetPosition(pos);
}

bool Cursor::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void Cursor::Shutdown()
{

}

void Cursor::SetOffset(XMFLOAT2 offset)
{
	m_offset = offset;
}