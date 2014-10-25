//--------------------------------------------------
//bomberman.cpp
//--------------------------------------------------
//Define Bomberman class

#include "bomberman.h"

Bomberman::Bomberman()
{
	m_pLevelData = 0;
	m_pDevice = 0;
	
	m_bombTimer = BOMB_TIME;
	m_activeBombs = 0;

	m_nextFrame = 0;	
	
	m_xVelocity = 0;
	m_yVelocity = 0;

	m_boundsMod = 0.66f;
	m_hit = false;
	m_godMode = true;
	m_godTimer = 0;

	m_fade = true;
	m_godAlpha = 1;
}

Bomberman::~Bomberman()
{
	Shutdown();
}

bool Bomberman::Initialize(ID3D11Device* device, XMFLOAT2 imageSize, XMFLOAT2 index, shared_ptr<LevelData> levelData)
{
	bool result;
	m_pDevice = device;
	m_pLevelData = levelData;
	m_index = index;
	m_spawnIndex = m_index;

	//Initialize the base 2d Object
	result = Base2D::Initialize(device, imageSize.x * ASPECT.x, imageSize.y * ASPECT.y, levelData->GetPosition(m_index.x, m_index.y), 0);
	if(!result)
		return false;

	//Load the Texture
	result = LoadTexture(device, StringManager::Bomberman());
	if(!result)
		return false;

	//Initialize the Bomberman animation
	m_pAnimation = shared_ptr<BombermanAnimation>(new BombermanAnimation);
	result = m_pAnimation->Initialize();
	if(!result)
		return false;

	//Create and Set the Collision Bounding Box
	m_pBox = shared_ptr<BoundingBox>(new BoundingBox);
	m_pBox->Set(m_imageWidth * m_boundsMod, m_imageHeight * m_boundsMod, POSITION);
	
	//Add A Bomb to the characters bomb total
	AddBomb();

	return true;
}

void Bomberman::Update(float gameTime)
{
	m_hit = false;
	GodMode(gameTime);

	//Two Speed Variables based on the Speeds and Aspect Ratio
	float speedX = Stats::GetSpeed() * ASPECT.x;
	float speedY = Stats::GetSpeed() * ASPECT.y;

	//Find bombermans tile index
	m_pLevelData->GetClosestIndex(POSITION, m_index);

	//Get Current Input Values
	int input = Input::GetCurrentKeys();
	int arrows = Input::GetCurrentSpecial();
	int down = Input::GetDownSpecial();

	//Process input into movement and play the corresponding animation
	//Move the objects towards the adjacent tiles
	if(input & KEY_W || arrows & KEY_UP)
	{		
		Move(speedY * gameTime, m_pLevelData->GetNode(m_index.x, m_index.y - 1));

		m_pAnimation->PlayBack();
	}
	else if(input & KEY_S || arrows & KEY_DOWN)
	{		
		Move(speedY * gameTime, m_pLevelData->GetNode(m_index.x, m_index.y + 1));
			
		m_pAnimation->PlayForward();
	}
	else if(input & KEY_A || arrows & KEY_LEFT)
	{
		Move(speedX * gameTime, m_pLevelData->GetNode(m_index.x - 1, m_index.y));

		m_pAnimation->PlayLeft();
	}
	else if(input & KEY_D || arrows & KEY_RIGHT)
	{
		Move(speedX * gameTime, m_pLevelData->GetNode(m_index.x + 1, m_index.y));
		
		m_pAnimation->PlayRight();
	}
	else
	{
		//Reset the values and play Idle - Prevent the character from moving with no input provided
		m_xVelocity = 0;
		m_yVelocity = 0;
		m_pAnimation->PlayIdle();
	}

	//Get the position of the current tile based on bombermans position
	XMFLOAT2 tPos = m_pLevelData->GetPosition(m_index.x, m_index.y);

	//Bomb timer - Prevents the placement of multiple bombs with one input
	m_bombTimer += gameTime;
	if(m_bombTimer > BOMB_TIME)
	{
		//Use the Down key input - prevent the use of multiple bombs in the same location
		if(down & KEY_SPACE)
			PlaceBomb(tPos);//Place the bomb in the current tile
	}

	//Update Bombs
	for(int c = 0; c < m_pBombs.size(); c++)
	{
		m_pBombs[c]->Update(gameTime);

		//Bomberman collision with the bomb
		int side = -1;
		if(m_pBombs[c]->IsPlanted())
		{
			if(this->CheckIntersect(m_pBombs[c]->GetBox(), side))
			{
				//Make sure that that player isnt still on the same tile as the bomb, if true ignore collision
				if(!XMVector2Equal(XMLoadFloat2(&this->m_index), XMLoadFloat2(&m_pBombs[c]->GetIndex())))
					this->CollisionWithWall(side,m_pBombs[c]->GetBox());
			}
		}
		else if(m_pBombs[c]->IsBoom())
		{
			//Check if the Fire has hit the player
			if(m_pBombs[c]->CheckFireBounds(this->GetBox()))
				Hit();

			//Check to see if the fire is hitting other bombs
			for(int d = 0; d < m_pBombs.size(); d++)
			{
				//Don't Check itself
				if(d == c)
					continue;

				//Ignore bombs that have exploded
				if(m_pBombs[d]->IsPlanted())
				{
					//Check the bounding boxes
					if(m_pBombs[c]->CheckFireBounds(m_pBombs[d]->GetBox()))
					{
						//Make the planted bomb explode
						m_pBombs[d]->Boom();

						m_activeBombs--;
						if(m_activeBombs < 0)
							m_activeBombs = 0;
					}
				}
			}
		}

		//Blow up the bomb and remove the bomb from active list (This will trigger once the timer on the bomb has been exceeded)
		if(m_pBombs[c]->RemoveFromCount())
		{
			m_pBombs[c]->Boom();

			m_activeBombs--;
			if(m_activeBombs < 0)
				m_activeBombs = 0;
		}

		//Remove the bomb completly
		if(!m_pBombs[c]->IsFinished())
		{
			m_pLevelData->ModifyPath(m_pBombs[c]->GetIndex().x, m_pBombs[c]->GetIndex().y, ACTIVE_NODE); 
			m_pBombs.erase(m_pBombs.begin() + c);
		}
	}
		
	//Bomberman collition with the Level 
	int side = -1;//Used to determine which side of the environment was hit
	for(int c = 0; c < m_pLevelData->GetGridY(); c++)
	{
		for(int d = 0; d < m_pLevelData->GetGridX(); d++)
		{
			if(this->CheckIntersect(m_pLevelData->GetNode(d,c)->GetBox(),side))
			{
				this->CollisionWithWall(side,m_pLevelData->GetNode(d,c)->GetBox());
				break;
			}
		}
	}

	//Animation Update
	m_nextFrame += gameTime;
	if(m_nextFrame > 180)
	{
		m_nextFrame = 0;
		m_pAnimation->NextFrame();
	}
	
	//Update bombermans position
	POSITION.x += m_xVelocity;
	POSITION.y += m_yVelocity;
}

//Release Memory Controlled objects
void Bomberman::Shutdown()
{
	if(m_pAnimation)
	{
		m_pAnimation.reset();
		m_pAnimation = 0;
	}

	if(m_pDevice)
		m_pDevice = 0;

	for(int c = 0; c < m_pBombs.size(); c++)
	{
		m_pBombs[c].reset();
		m_pBombs[c] = 0;
	}
	m_pBombs.clear();

	if(m_pLevelData)
	{
		m_pLevelData.reset();
		m_pLevelData = 0;
	}
}

bool Bomberman::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	for(int c = 0; c < m_pBombs.size(); c++)
	{
		//Render the bomb
		result = m_pBombs[c]->Render(context, shader, world, view, ortho);
		if(!result)
			return false;	
	}

	//Render Bomberman
	result = Base2D::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

//Move bombermand closer to the next tile
void Bomberman::Move(float d, Node* tile)
{
	XMFLOAT2 pos;
	float x;
	float y;

	//Get the position of the tile and distance between it and bomberman
	pos = tile->GetPosition();
	x = pos.x - POSITION.x;
	y = pos.y - POSITION.y;

	//Normalize the value
	XMVECTOR t = XMVector2Normalize(XMLoadFloat2(&XMFLOAT2(x,y)));

	x = XMVectorGetX(t);
	y = XMVectorGetY(t);

	//Update Velocity
	m_xVelocity = (d * x);
	m_yVelocity = (d * y);
}

//Move bombermand using a set direction
void Bomberman::Move(float d, XMFLOAT2 dir)
{
	//Track the current Rotation - Never Modified in this project
	float S = sin(ROTATION);
	float C = cos(ROTATION);

	//Do Sine + Cosn Calculations - will move based on the objects current rotation
	float x = (dir.x * C) + (dir.y * S);
	float y = (dir.x * S) - (dir.y * C);

	//Update the Velocity
	m_xVelocity = (d * x);
	m_yVelocity = (d * y);
}

//Rotate this object
void Bomberman::Rotate(float d) // <- Never used in this project
{
	float t = XMConvertToRadians(d);
	ROTATION += t;

	if(ROTATION > 360)
		ROTATION -= 360;
	else if(ROTATION < -360)
		ROTATION += 360;
}

//Update the Texture using the animation object
void Bomberman::UpdateTexture(VertexType* pVertex)
{
	pVertex[0].texture = XMFLOAT2(m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].U.x, m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].V.x);
	pVertex[1].texture = XMFLOAT2(m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].U.y, m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].V.y);
	pVertex[2].texture = XMFLOAT2(m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].U.x, m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].V.y);
	pVertex[3].texture = XMFLOAT2(m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].U.x, m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].V.x);
	pVertex[4].texture = XMFLOAT2(m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].U.y, m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].V.x);
	pVertex[5].texture = XMFLOAT2(m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].U.y, m_pAnimation->GetCurrent()->info[m_pAnimation->GetFrameNumber()].V.y);
}

//Update the velocity based on the distance of the penetration into the other object
void Bomberman::CollisionWithWall(int side, BoundingBox* box)
{
	float dist = 0;
	switch(side)
	{
	case 0://bottom
		dist = m_pBox->bottom - box->top;

		if(m_yVelocity < 0)
			m_yVelocity -= dist;
		break;
	case 1://top
		dist = m_pBox->top - box->bottom;

		if(m_yVelocity > 0)
			m_yVelocity -= dist;
		break;
	case 2://left
		dist = m_pBox->left - box->right;

		if(m_xVelocity < 0)
			m_xVelocity -= dist;
		break;
	case 3://right
		dist = m_pBox->right - box->left;

		if(m_xVelocity > 0)
			m_xVelocity -= dist;
		break;
	default:
		break;
	}
}

XMFLOAT2 Bomberman::GetIndex()
{
	return m_index;
}
bool Bomberman::GetHit()
{
	//Put the death code here
	return m_hit;
}

void Bomberman::PlaceBomb(const XMFLOAT2& pos)
{
	bool result = false;
	int c;
	
	//Check the Current Active Bombs vs the Allowed active bombs
	if(m_activeBombs < Stats::GetBombCount())
	{
		for(c = 0; c < m_pBombs.size(); c++)
		{
			//Check to see if any current active bomb(still inPlanted State) is currently in the desired position
			if(m_pBombs[c]->IsPlanted())
			{
				if(XMVector2Equal(XMLoadFloat2(&m_pBombs[c]->GetPosition()), XMLoadFloat2(&pos)))
				{
					//Bomb was found in the same position
					result = true;
					break;
				}
			}
		}

		if(!result)
		{
			//Create a new bomb and place it at this position
			m_pBombs.push_back(shared_ptr<Bomb>(new Bomb));
			m_pBombs[m_pBombs.size() - 1]->Initialize(m_pDevice, XMFLOAT2(50 * ASPECT.x,50 * ASPECT.y), m_pLevelData, m_index, Stats::GetBombSpread());
			m_activeBombs++;
			m_bombTimer = 0;
			m_pLevelData->ModifyPath(m_index.x, m_index.y, INACTIVE_NODE);
		}
	}
}

void Bomberman::AddBomb()
{
	Stats::IncreaseBombCount();
}

void Bomberman::CheckEnemyIntersects(Enemy* enemy)
{
	int side = -1;

	//have bomberman and an enemy collided?
	if(this->CheckIntersect(enemy->GetBox(), side))
		Hit();

	for(int c = 0; c < m_pBombs.size(); c++)
	{
		if(m_pBombs[c]->IsBoom())
		{
			//Check if the Fire has hit the player
			if(m_pBombs[c]->CheckFireBounds(enemy->GetBox()))
			{
				enemy->Hit();
				Stats::IncreaseScore(100);
				break;
			}
		}
	}
}

//Hit behaviour
void Bomberman::Hit()
{
	if(!m_godMode)
	{
		m_hit = true;
		m_index = m_spawnIndex;
		POSITION = m_pLevelData->GetPosition(m_index.x, m_index.y);
		Stats::DecreaseLives();
		m_godMode = true;

		if(!m_cameraCallback._Empty())
			m_cameraCallback();
	}
}

//God Mode Behaviour
void Bomberman::GodMode(float gameTime)
{
	if(m_godMode)
	{
		//Fade the texture in and out displaying the god mode state
		GodModeFade(gameTime);
		m_godTimer += gameTime;
		if(m_godTimer > GOD_MODE_TIME)
		{
			if(!_GOD_MODE)
				m_godMode = false;

			Base2D::m_colorOverride.w = m_godAlpha = 1;
			m_godTimer = 0;
		}
	}
}
void Bomberman::GodModeFade(float gameTime)
{
	float speed = gameTime / 1000;
	//Fade the character in and out using the Textures Alpha
	if(m_fade)
	{
		m_godAlpha += speed;
		if(m_godAlpha > 1)
		{
			m_godAlpha = 1;
			m_fade = false;
		}
	}
	else
	{
		m_godAlpha -= speed;
		if(m_godAlpha < 0.5f)
		{
			m_godAlpha = 0.5f;
			m_fade = true;
		}
	}

	Base2D::m_colorOverride.w = m_godAlpha;
}

void Bomberman::SetCameraCallback(function<void(void)> callback)
{
	m_cameraCallback = callback;
}