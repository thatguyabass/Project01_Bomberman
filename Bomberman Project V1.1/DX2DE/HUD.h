//------------------------------------------------------------------------------------------------
//HUD.h
//------------------------------------------------------------------------------------------------
//This class will display the HUD to the player during gameplay

#ifndef _HUD_H_
#define _HUD_H_

#include "Global.h"

#include "font.h"
#include "text.h"
#include "static2D.h"

#include "stringManager.h"

#include "fps.h"
#include "stats.h"

//-----------------------------------------------------------------------------------------------

#define STATS Stats::STATS

#define FPS_INDEX 0
#define LIVES_INDEX 1
#define SCORE_INDEX 2

class HUD
{
public:
	HUD();
	HUD(const HUD&);
	~HUD();

	bool Initialize(ID3D11Device*);
	void Update(XMFLOAT2);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	Font* GetFont();

private:
	//Methods
	void Shutdown();
	bool InitializeFont(ID3D11Device*);

	void UpdateLives();	//Update the Lives Text
	void UpdateScore();	//Update the Score Text
	void UpdateFPS();	//Updat the FPS Text
	
	//Variables
	shared_ptr<Font> m_pFont;

	int m_lives;
	int m_score;

	Static2D m_background;
	vector<shared_ptr<Text>> m_pText;

};


#endif//_HUD_H_