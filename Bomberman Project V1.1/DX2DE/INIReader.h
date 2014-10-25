//---------------------------------------------------------
//INIReader.h
//---------------------------------------------------------
//Read and store the values from an INI file

#ifndef _INI_READER_H_
#define _INI_READER_H_

#include<string>
#include<map>
#include<fstream>
#include<Windows.h>
using namespace std;

class INIReader
{
public:
	INIReader(wstring fileName);

	string Get(wstring section, wstring key, wstring defaultValue);
	int GetInt(wstring section, wstring key, int defaultValue);
	bool GetBool(wstring section, wstring key, bool defaultValue);
	float GetFloat(wstring section, wstring key, float defaultValue); 

private:
	//Variables
	wstring m_fileName;

};

#endif _INI_READER_H_