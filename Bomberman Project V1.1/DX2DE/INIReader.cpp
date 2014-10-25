//--------------------------------------------------
//INIReader.cpp
//--------------------------------------------------
//Read the file using the section and the key to return the value

#include "INIReader.h"

INIReader::INIReader(wstring fileName)
{
	m_fileName = fileName;
}

string INIReader::Get(wstring section, wstring key, wstring defaultValue)
{
	TCHAR value[32];
	GetPrivateProfileString(section.c_str(), key.c_str(), L"" , value, 32, m_fileName.c_str());

	wstring ws = value;
	if(ws.empty())
		ws = defaultValue;
	
	string s(ws.begin(), ws.end());
	for(int c = 0; c < s.length(); c++)
		s[c] = tolower(s[c]);

	return s;
}

int INIReader::GetInt(wstring section, wstring key, int defaultValue)
{
	string get = Get(section, key, L"");
	int value;
	if(get.empty())
		value = defaultValue;
	else
		value = stoi(get);

	return value;
}

bool INIReader::GetBool(wstring section, wstring key, bool defaultValue)
{
	string get = Get(section, key, L"");
	if(get == "false" || get == "0")
		return false;
	else if(get == "true" || get == "1")
		return true;
	else
		return defaultValue;
}

float INIReader::GetFloat(wstring section, wstring key, float defaultValue)
{
	string get = Get(section, key, L"");
	float value;
	if(get.empty())
		value = defaultValue;
	else
		value = stof(get);

	return value;
}