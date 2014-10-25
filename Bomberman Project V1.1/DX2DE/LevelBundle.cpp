//--------------------------------------------------------------------------
//LevelBundle.cpp
//--------------------------------------------------------------------------
//

#include "LevelBundle.h"

LevelBundle::LevelBundle()
{
	m_levelIndex = 0;
	m_continue = true;
}

LevelBundle::~LevelBundle()
{
	Shutdown();
}

void LevelBundle::Shutdown()
{
	m_levels.clear();
}

//Convert the string into a wstring
void LevelBundle::Initialize(string path)
{
	path = StringManager::BundleDir() + path;// + StringManager::LevelExtention();
	wstring bundle(path.begin(), path.end());
	Initialize(bundle);
}
//Clear any current bundle and read the file
void LevelBundle::Initialize(wstring path)
{
	m_levelIndex = 0;
	m_levels.clear();
	m_continue = true;

	ReadFile(path);
}

//Return the current level
wstring LevelBundle::GetLevel()
{
	return m_levels[m_levelIndex];
}
//Return the list of levels
vector<wstring>& LevelBundle::GetLevels()
{
	return m_levels;
}

//Increment the next level
void LevelBundle::NextLevel()
{
	m_levelIndex++;
	if(m_levelIndex >= m_levels.size() - 1)
		m_continue = false;
}

bool LevelBundle::GetContinue()
{
	return m_continue;
}

bool LevelBundle::ReadFile(wstring dir)
{
	ifstream in;
	string line;
	int count;

	in.open(dir.c_str());

	if(in.fail())
		return false;

	//Get the total number of levels
	in >> count;

	//Read the end of the last line
	in.get();
	for(int c = 0; c < count; c++)
	{
		getline(in,line, '\n');
		m_levels.push_back(wstring(line.begin(), line.end()));
	}

	in.close();

	return true;
}

bool LevelBundle::WriteFile(string name, vector<string>& bundle)
{
	ofstream out;

	//Convert the name into a wstring for the file path
	string file = StringManager::BundleDir() + name + StringManager::LevelExtention();
	wstring wFile(file.begin(), file.end());

	out.open(wFile);

	//Write out how many levels are bundled
	out << bundle.size() << endl;

	for(int c = 0; c < bundle.size(); c++)
		out << StringManager::CustomLevelDir() << bundle[c] /*<< StringManager::LevelExtention()*/ << endl;

	out.close();

	return true;
}