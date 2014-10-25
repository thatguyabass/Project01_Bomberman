//-----------------------------------------------------------------
//stats.h
//-----------------------------------------------------------------
//define the stats that bomberman will use through a game

#ifndef _STATS_H_
#define _STATS_H_

static class Stats
{
public:
	Stats();
	~Stats();

	static void IncreaseLives();
	static void DecreaseLives();
	static void SetLives(int);
	static int GetLives();
	static bool BomberDead();
	
	static void IncreaseBombCount();
	static void DecreaseBombCount();
	static void SetBombCount(int);
	static int GetBombCount();

	static void IncreaseBombSpread();
	static void DecreaseBombSpread();
	static void SetBombSpread(int);
	static int GetBombSpread();

	static void IncreaseScore(int modifier);
	static void DecreaseScore(int modifier);
	static void SetScore(int);
	static int GetScore();

	static void IncreaseSpeed(float);
	static void DecreaseSpeed(float);
	static void SetSpeed(float);
	static float GetSpeed();

	static void Reset();

private:
	//Variables
	static int m_lives;
	static int m_bombCount;
	static int m_bombSpread;
	static int m_score;

	static float m_speed;
	static float m_maxSpeed;
	static float m_baseSpeed; 

	static const int MAX_LIVES = 99;
	static const int MAX_BOMB_COUNT = 10;
	static const int MAX_BOMB_SPREAD = 10;

	static const int MIN_BOMB_COUNT = 1;
	static const int MIN_BOMB_SPREAD = 2;

	static const int BASE_LIVES = 5;
	static const int BASE_SCORE = 0;
};

#endif//_STATS_H_