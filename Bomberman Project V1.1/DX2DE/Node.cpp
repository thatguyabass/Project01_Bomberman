//-------------------------------------------------
//tile.cpp
//-------------------------------------------------
//Define the Tile class 

#include "Node.h"

Node::Node()
{
	m_pAlt = 0;
	m_pFText = 0;
}

Node::~Node()
{
	Shutdown();
}

bool Node::Initialize(ID3D11Device* device, int imageWidth, int imageHeight, char tileType, XMFLOAT2 position, int id, XMFLOAT2 index, XMFLOAT2& spawn, XMFLOAT2& exit)
{
	bool result;
	TileCharacter tileChar;//Holds the Data to create the node

	//Establish this tiles position on the grid
	m_id = id;
	m_index = index;

	//Initialize the image
	result = Base2D::Initialize(device, imageWidth, imageHeight, position, 0);
	if(!result)
		return false;

	//Load Base Texture
	result = Base2D::LoadTexture(device, StringManager::Node());
	if(!result)
		return false;

	//Set the Tile State
	switch(tileType)
	{
	case 'o': e_tile = SPACE;			break;
	case 'x': e_tile = NO_SPACE;		break;
	case 'y': e_tile = DESTRUCTIBLE;	break;
	case 'e': e_tile = EXIT;			break;
	case 's': e_tile = SPAWN;			break;
	default:  e_tile = SPACE;			break;
	}
	tileChar.SetChar(e_tile);//Set the Tile Character using the Tile State

	//Load Base Texture
	result = Base2D::LoadTexture(device, tileChar.Texture());
	if(!result)
		return false;

	//Load the Additional Texture
	if(tileChar.CheckAlt())
	{
		m_pAlt = shared_ptr<Texture>(new Texture);
		result = m_pAlt->Initialize(device, tileChar.Alt());
		if(!result)
			return false;
	}

	//Create a Bounding Box
	if(tileChar.BoundingBox())
	{
		m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
		m_pBox->Set(imageWidth, imageHeight, position);
	}

	//Set the Exit
	if(tileChar.Exit())
	{
		e_tile = DESTRUCTIBLE;
		exit = m_index;
	}

	//Set the Spwan
	if(tileChar.Spawn())
	{
		e_tile = SPACE;
		spawn = m_index;
	}

	if(_DEBUG_PATHFINDING)
	{
		m_pNode = shared_ptr<Static2D>(new Static2D);
		result = m_pNode->Initialize(device, StringManager::Node(), imageWidth / 5, imageHeight /5, position, 0);
		if(!result)
			return false;
	}

	//Set the Node State
	switch(tileType)
	{
	case 'y':
	case 'e':
	case 'x':
		if(_DEBUG_PATHFINDING)
			m_pNode->SetColor(XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f));
		e_node = INACTIVE;
		break;
	default:
		if(_DEBUG_PATHFINDING)
			m_pNode->SetColor(Color::Blue());
		e_node = ACTIVE;
	}

	return true;
}

void Node::Update(float gameTime)
{

}

bool Node::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	return BaseNode::Render(context, shader, world, view, ortho); 
}

void Node::Shutdown()
{
	if(m_pAlt)
	{
		m_pAlt.reset();
		m_pAlt = 0;
	}
}

bool Node::Destroy()
{
	//Check the Tile State
	if(e_tile == DESTRUCTIBLE)
	{
		//Release the Current Texture and Bounding Box
		ReleaseTexture();
		m_pBox.reset();
		m_pBox = 0;

		//Set the Tile State
		e_tile = SPACE;
		m_pTexture = m_pAlt;//Set the texture to the current texture

		e_node = ACTIVE;
		
		return true;
	}
	return false;
}

bool Node::operator==(Node& node)
{
	if(this == &node)
		return true;
	return false;
}
bool Node::operator==(const Node& node)
{
	if(this == &node)
		return true;
	return false;
}

bool Node::InitializeText(ID3D11Device* device, shared_ptr<Font> font, int width, int height, HWND hwnd, float scale)
{
	bool result;

	//Font Position is based off the Top left corner
	//Get the Position based on the Nodes Position on the Screen
	int X = POSITION.x;
	int _X = (width / 2) - X;
	X *= 2;
	X += _X;

	int Y = POSITION.y;
	Y = (height / 2) - Y;

	m_pFText = shared_ptr<Text>(new Text);
	if(!m_pFText)
		return false;

	result = m_pFText->Initialize(device, font, "F", 100, XMFLOAT2(width + -X + 30, Y - 40), Color::White(), scale);
	if(!result)
		return false;
	
	m_pHText = shared_ptr<Text>(new Text);
	result = m_pHText->Initialize(device, font, "H", 100, XMFLOAT2(width + -X -20, Y - 40), Color::White(), scale);
	if(!result)
		return false;

	m_pGText = shared_ptr<Text>(new Text);
	result = m_pGText->Initialize(device, font, "G", 100, XMFLOAT2(width + -X - 20, Y + 10), Color::White(), scale);
	if(!result)
		return false;

	return true;
}

void Node::UpdateText(float gameTime)
{
	if(m_F != 0)
		m_pFText->SetText(std::to_string(m_F));

	if(m_H != 0)
		m_pHText->SetText(std::to_string(m_H));

	if(m_G != 0)
		m_pGText->SetText(std::to_string(m_G));
}

bool Node::RenderText(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;
	if(m_F != 0)
	{
		result = m_pFText->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	if(m_H != 0)
	{
		result = m_pHText->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	if(m_G != 0)
	{
		result = m_pGText->Render(context, shader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

Static2D* Node::GetDebugNode()
{
	return m_pNode.get();
}