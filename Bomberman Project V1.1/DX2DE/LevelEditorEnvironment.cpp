//-----------------------------------------------------
//LevelEditorEnvironment.cpp
//-----------------------------------------------------
//

#include "LevelEditorEnvironment.h"

LevEdEnvironment::LevEdEnvironment()
{
	e_level = NONE;
}

LevEdEnvironment::~LevEdEnvironment()
{
	Shutdown();
}

bool LevEdEnvironment::Initialize(ID3D11Device* device, shared_ptr<Font> font)
{
	bool result;

	//Button Dimensions
	float buttonWidth = 50;
	float buttonHeight = 50;

	//X Positions
	float leftPos = 460 * ASPECT.x;
	float middlePos = 515 * ASPECT.x;
	float rightPos = 570 * ASPECT.x;

	//Create and Initialize the Text
	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[0]->Initialize(device, font, "Tiles", 12, XMFLOAT2(480 * ASPECT.x, 200 * ASPECT.y), Color::White(), 1.5f);
	if(!result)
		return false;

	m_pText.push_back(shared_ptr<Text>(new Text));
	result = m_pText[1]->Initialize(device, font, "Enemies", 15, XMFLOAT2(460 * ASPECT.x, 35 * ASPECT.y), Color::White(), 1.5f);
	if(!result)
		return false;
	
	//Create and Initialize the Buttons
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[0]->Initialize(device, &StringManager::TileSpace(), buttonWidth, buttonHeight, XMFLOAT2(leftPos, 140 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[1]->Initialize(device, &StringManager::TileNoSpace(), buttonWidth, buttonHeight, XMFLOAT2(middlePos, 140 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[2]->Initialize(device, &StringManager::TileRemovable(), buttonWidth, buttonHeight, XMFLOAT2(rightPos, 140 * ASPECT.y), ONCE);
	if(!result)
		return false;
	
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[3]->Initialize(device, &StringManager::TileExit(), buttonWidth, buttonHeight, XMFLOAT2(475 * ASPECT.x, 80 * ASPECT.y), ONCE);
	if(!result)
		return false;
	
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[4]->Initialize(device, &StringManager::TileSpawn(), buttonWidth, buttonHeight, XMFLOAT2(555 * ASPECT.x, 80 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[5]->Initialize(device, &StringManager::TileHeli(), buttonWidth, buttonHeight, XMFLOAT2(leftPos, -20 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[6]->Initialize(device, &StringManager::TileYellow(), buttonWidth, buttonHeight, XMFLOAT2(middlePos, -20 * ASPECT.y), ONCE);
	if(!result)
		return false;
	
	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[7]->Initialize(device, &StringManager::TileMadGuy(), buttonWidth, buttonHeight, XMFLOAT2(rightPos, -20 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[8]->Initialize(device, &StringManager::TileMadBomb(), buttonWidth, buttonHeight, XMFLOAT2(leftPos, -80 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[9]->Initialize(device, &StringManager::TileRabbit(), buttonWidth, buttonHeight, XMFLOAT2(middlePos, -80 * ASPECT.y), ONCE);
	if(!result)
		return false;

	m_pButtons.push_back(shared_ptr<Button>(new Button));
	result = m_pButtons[10]->Initialize(device, &StringManager::TileBlueEye(), buttonWidth, buttonHeight, XMFLOAT2(rightPos, -80 * ASPECT.y), ONCE);
	if(!result)
		return false;
	
	//Set up the button callbacks
	m_pButtons[0]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), SPACE);
	m_pButtons[1]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), NO_SPACE);
	m_pButtons[2]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), DESTRUCTIBLE);
	m_pButtons[3]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), EXIT);
	m_pButtons[4]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), SPAWN);
	m_pButtons[5]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), HELI);
	m_pButtons[6]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), YELLOW);
	m_pButtons[7]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), MAD_GUY);
	m_pButtons[8]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), MAD_BOMB);
	m_pButtons[9]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), RABBIT);
	m_pButtons[10]->SetFunction(std::bind(&LevEdEnvironment::SetState, this, _1), BLUE_EYE);

	//Create and initialize the Toggle Objects
	m_pToggle = shared_ptr<Static2D>(new Static2D);
	result = m_pToggle->Initialize(device, StringManager::Connection(), buttonWidth * ASPECT.x, buttonHeight * ASPECT.y, XMFLOAT2(0,0), 0);
	if(!result)
		return false;

	XMFLOAT4 gray = Color::Gray();
	m_pToggle->SetColor(XMFLOAT4(gray.x, gray.y, gray.z, 0.5f)); 

	return true;
}

void LevEdEnvironment::Update(float gameTime, XMFLOAT2 offset)
{
	//Set the Offset and Update each object for proper placement when moving the camera
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c]->SetOffset(offset);
		m_pText[c]->Update();
	}

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c]->SetOffset(offset);
		m_pButtons[c]->Update(gameTime);
	}

	//Move the position of the toggle object to the currently selected button
	switch(e_level)
	{
	case NONE: break;
		default: 
			m_pToggle->SetPosition(m_pButtons[e_level - 1]->GetPosition());
			m_pToggle->SetOffset(offset);
			break;
	}
}

bool LevEdEnvironment::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> textureShader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pButtons.size(); c++)
	{
		result = m_pButtons[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	for(int c = 0; c < m_pText.size(); c++)
	{
		result = m_pText[c]->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	if(e_level != NONE)
	{
		result = m_pToggle->Render(context, textureShader, world, view, ortho);
		if(!result)
			return false;
	}

	return true;
}

void LevEdEnvironment::Shutdown()
{
	//Release the Text
	for(int c = 0; c < m_pText.size(); c++)
	{
		m_pText[c].reset();
		m_pText[c] = 0;
	}
	m_pText.clear();

	//Release the Buttons
	for(int c = 0; c < m_pButtons.size(); c++)
	{
		m_pButtons[c].reset();
		m_pButtons[c] = 0;
	}
	m_pButtons.clear();

	//Release the Toggle Object
	if(m_pToggle)
	{
		m_pToggle.reset();
		m_pToggle = 0;
	}
}

int LevEdEnvironment::GetState()
{
	return e_level;
}
//Set the Level State
void LevEdEnvironment::SetState(int value)
{
	if(value == e_level)
		value = 0;

	switch(value)
	{
	case 0:		e_level = NONE;			break;
	case 1:		e_level = SPACE;		break;
	case 2:		e_level = NO_SPACE;		break;
	case 3:		e_level = DESTRUCTIBLE; break;
	case 4:		e_level = EXIT;			break;
	case 5:		e_level = SPAWN;		break;
	case 6:		e_level = HELI;			break;
	case 7:		e_level = YELLOW;		break;
	case 8:		e_level = MAD_GUY;		break;
	case 9:		e_level = MAD_BOMB;		break;
	case 10:	e_level = RABBIT;		break;
	case 11:	e_level = BLUE_EYE;		break;
	default:	e_level = NONE;			break;
	}
}