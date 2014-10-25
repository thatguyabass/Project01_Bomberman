//-------------------------------------------------------------
//stringManager.h
//-------------------------------------------------------------
//this class will contain all the strings in the game

#ifndef _STRING_MANAGER_H_
#define _STRING_MANAGER_H_

#include<string>
#include<memory>
using namespace std;

static class StringManager
{
public:
	StringManager() {};
	~StringManager() {};

	//Bomberman
	static wstring Bomberman() { return L"../DX2DE/Textures/Bomberman/Bomberman.png"; }
	static wstring Bomb() { return L"../DX2DE/Textures/Bomberman/Bomb.png"; }
	static wstring Fire() { return L"../DX2DE/Textures/Fire/Center.png"; }

	//Level Tiles
	static wstring TileSpace() { return L"../DX2DE/Textures/Level/Space.png"; }
	static wstring TileNoSpace() { return L"../DX2DE/Textures/Level/NoSpace.png"; }
	static wstring TileRemovable() { return L"../DX2DE/Textures/Level/Remove.png"; }
	static wstring TileExit() { return L"../DX2DE/Textures/Level/Exit.png"; }
	static wstring TileSpawn() { return L"../DX2DE/Textures/Level/Spawn.png"; }

	//Editor specific
	static wstring TileHeli() { return L"../DX2DE/Textures/Editor/HeliTile.PNG"; }
	static wstring TileYellow() { return L"../DX2DE/Textures/Editor/YellowTile.PNG"; }
	static wstring TileBlueEye() { return L"../DX2DE/Textures/Editor/BlueEyeTile.PNG"; }
	static wstring TileMadBomb() { return L"../DX2DE/Textures/Editor/MadBombTile.PNG"; }
	static wstring TileRabbit() { return L"../DX2DE/Textures/Editor/RabbitTile.PNG"; }
	static wstring TileMadGuy() { return L"../DX2DE/Textures/Editor/MadGuyTile.PNG"; }
	static wstring LevEdBackground() { return L"../DX2DE/Textures/Editor/LevEdBackground.png"; }

	//Enemies
	static wstring Heli() { return L"../DX2DE/Textures/Enemy/Heli.PNG"; }
	static wstring Yellow() { return L"../DX2DE/Textures/Enemy/Yellow.PNG"; }
	static wstring MadBomb() { return L"../DX2DE/Textures/Enemy/MadBomb.PNG"; }
	static wstring BlueEye() { return L"../DX2DE/Textures/Enemy/BlueEye.PNG"; }
	static wstring Rabbit() { return L"../DX2DE/Textures/Enemy/Rabbit.PNG"; }
	static wstring MadGuy() { return L"../DX2DE/Textures/Enemy/MadGuy.PNG"; }

	//Font
	static wstring FontTexture() { return L"../DX2DE/Font/Font.PNG"; }
	static wstring InvertFontTexture() { return L"../DX2DE/Font/WhiteFont.PNG"; }
	static char* FontText() { return "../DX2DE/Font/Font.txt"; }

	//Levels
	static wstring Level1() { return L"../DX2DE/Levels/Campaign/Level1.txt"; }
	static wstring Level2() { return L"../DX2DE/Levels/Campaign/Level2.txt"; }
	static wstring Level3() { return L"../DX2DE/Levels/Campaign/Level3.txt"; }
	static wstring Level4() { return L"../DX2DE/Levels/Campaign/Level4.txt"; }
	static wstring Level5() { return L"../DX2DE/Levels/Campaign/Level5.txt"; }

	//Directories
	static wstring LevelDirectory() { return L"..\\DX2DE\\Levels\\*.txt"; } 
	static wstring CustomLevelDirectory() { return L"..\\DX2DE\\Levels\\Custom\\*.txt"; } 
	static string CustomLevelDir() { return "../DX2DE/Levels/Custom/"; }
	static string BundleDir() { return "../DX2DE/Levels/Bundle/"; }
	static wstring BundleDirectory() { return L"..\\DX2DE\\Levels\\Bundle\\*.txt"; } 
	static wstring CampaignBundle() { return L"../DX2DE/Levels/Campaign/Campaign.txt"; }
	static string LevelExtention() {return ".txt"; }

	//Pathfinding
	static wstring Node() { return L"../DX2DE/Textures/Pathfinding/Node.png"; }
	static wstring Connection() { return L"../DX2DE/Textures/Pathfinding/Connection.png"; }
	static wstring Seeker() { return L"../DX2DE/Textures/Pathfinding/Seeker.png"; }

	//GUI
	static wstring GUIAdd() { return L"../DX2DE/Textures/GUI/Add.png"; }
	static wstring GUIRemove() { return L"../DX2DE/Textures/GUI/Remove.png"; }
	static wstring GUIUpArrow() { return L"../DX2DE/Textures/GUI/UpArrow.png"; }
	static wstring GUIDownArrow() { return L"../DX2DE/Textures/GUI/DownArrow.png"; }
	static wstring GUICancel() { return L"../DX2DE/Textures/GUI/Cancel.png"; }
	static wstring GUIBlank() { return L"../DX2DE/Textures/GUI/Blank.png"; }

	//Menus
	static wstring MainMenu() { return L"../DX2DE/Textures/Misc/MainMenu.jpg"; }
	static wstring Continue() { return L"../DX2DE/Textures/Misc/BombermanContinue.jpg"; }
	static wstring GameOver() { return L"../DX2DE/Textures/Misc/BombermanGameOver.jpg"; }
	static wstring LevelSelect() { return L"../DX2DE/Textures/Misc/LevelSelect.jpg"; }
	static wstring BundleSelect() { return L"../DX2DE/Textures/Misc/BundleSelect.jpg"; }

	//Power ups
	static wstring PowerUpCount() { return L"../DX2DE/Textures/Power/BombCount.PNG"; }
	static wstring PowerUpSpread() { return L"../DX2DE/Textures/Power/FireSpread.PNG"; }
	static wstring PowerUpExtraLife() { return L"../DX2DE/Textures/Power/ExtraLife.PNG"; }
	static wstring PowerUpSpeed() { return L"../DX2DE/Textures/Power/Speed.PNG"; }
	
	//Config
	static wstring ConfigPath() { return L"..\\DX2DE\\Config.ini"; }
	static wstring ConfigGraphicSection() { return L"Graphic"; }
	static wstring ConfigWidth() { return L"Width"; }
	static wstring ConfigHeight() { return L"Height"; }
	static wstring ConfigVSync() { return L"V-Sync"; }
	static wstring ConfigFullscreen() { return L"Fullscreen"; }
	static wstring ConfigGodMode() { return L"GodMode"; }

};

#endif//_STRING_MANAGER_H_