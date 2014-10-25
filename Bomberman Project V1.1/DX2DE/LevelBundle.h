//----------------------------------------------------------------
//LevelBundle.h
//----------------------------------------------------------------
//Use Level Bundles to store the file path of levels to be played in order
//This class will allow the user to create their own bundles allowing them to play their own custom campaigns

#ifndef _LEVEL_BUNDLE_H_
#define _LEVEL_BUNDLE_H_

#include "stringManager.h"

#include<vector>
#include<fstream>
using namespace std;

//---------------------------------------------------------------------------------------

class LevelBundle
{
public:
	LevelBundle();
	~LevelBundle();

	//Initialize will convert the string to a wstring and pass to the other Initialize Method
	void Initialize(string);
	void Initialize(wstring);
	//Write the bundle to a file to be used by the player
	bool WriteFile(string, vector<string>&);

	//Increment the current level index
	void NextLevel();
	//Get the current level
	wstring GetLevel();
	//Return a list of every level
	vector<wstring>& GetLevels();
	//Can the player continue after this level or is this the last level?
	bool GetContinue();

private:
	//Methods
	void Shutdown();
	//Read and populate the levels from a text file
	bool ReadFile(wstring);

	//Variables
	int m_levelIndex;
	vector<wstring> m_levels;
	bool m_continue;

};

#endif//_LEVEL_BUNDLE_H_