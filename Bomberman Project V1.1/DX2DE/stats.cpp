//--------------------------------------------------------------------
//stats.cpp
//--------------------------------------------------------------------
//Define the game states

#include "stats.h"

Stats::Stats()
{

}

Stats::~Stats()
{

}

//--------------------------VARIABLES---------------------------

int Stats::m_lives = Stats::BASE_LIVES;
int Stats::m_bombCount = Stats::MIN_BOMB_COUNT;
int Stats::m_score = Stats::BASE_SCORE;
int Stats::m_bombSpread = Stats::MIN_BOMB_SPREAD;

float Stats::m_baseSpeed = 0.15f;
float Stats::m_maxSpeed = 0.2f;
float Stats::m_speed = Stats::m_baseSpeed;

//--------------------------Life Methods---------------------------
void Stats::IncreaseLives() { SetLives(m_lives + 1); }
void Stats::DecreaseLives() { SetLives(m_lives - 1); }
int Stats::GetLives() { return m_lives; }
void Stats::SetLives(int count) 
{ 
	m_lives = count; 
	if(m_lives > MAX_LIVES)
		m_lives = MAX_LIVES;
	else if(m_lives < 0)
		m_lives = 0;
}
bool Stats::BomberDead()
{
	if(m_lives > 0)
		return false;
	else
		return true;
}

//--------------------------Bomb Methods---------------------------
void Stats::IncreaseBombCount() { SetBombCount(m_bombCount + 1); }
void Stats::DecreaseBombCount() { SetBombCount(m_bombCount - 1); }
int Stats::GetBombCount() { return m_bombCount; }
void Stats::SetBombCount(int count) 
{
	m_bombCount = count; 
	if(m_bombCount > MAX_BOMB_COUNT)
		m_bombCount = MAX_BOMB_COUNT;
	else if(m_bombCount < MIN_BOMB_COUNT)
		m_bombCount = MIN_BOMB_COUNT;
}

//--------------------------Bomb Spread Methods---------------------------
void Stats::IncreaseBombSpread() { SetBombSpread(m_bombSpread + 1); }
void Stats::DecreaseBombSpread() { SetBombSpread(m_bombSpread - 1); }
int Stats::GetBombSpread() { return m_bombSpread; }
void Stats::SetBombSpread(int spread) 
{ 
	m_bombSpread = spread; 
	if(m_bombSpread > MAX_BOMB_SPREAD)
		m_bombSpread = MAX_BOMB_SPREAD;
	else if(m_bombSpread < MIN_BOMB_SPREAD)
		m_bombSpread = MIN_BOMB_SPREAD;
}


//--------------------------Score Methods---------------------------
void Stats::IncreaseScore(int modifier) { SetScore(m_score + modifier); }
void Stats::DecreaseScore(int modifier) { SetScore(m_score - modifier); }
void Stats::SetScore(int score) { m_score = score; }
int Stats::GetScore() { return m_score; }

void Stats::IncreaseSpeed(float mod) { SetSpeed(m_speed + mod); }
void Stats::DecreaseSpeed(float mod) { SetSpeed(m_speed - mod); }
void Stats::SetSpeed(float speed) 
{
	m_speed = speed;
	if(m_speed > m_maxSpeed)
		m_speed = m_maxSpeed;
	else if(m_speed < m_baseSpeed)
		m_speed = m_baseSpeed; 
}
float Stats::GetSpeed() { return m_speed; }

void Stats::Reset()
{
	m_lives = BASE_LIVES;
	m_score = BASE_SCORE;
	m_bombCount = MIN_BOMB_COUNT;
	m_bombSpread = MIN_BOMB_SPREAD;
}