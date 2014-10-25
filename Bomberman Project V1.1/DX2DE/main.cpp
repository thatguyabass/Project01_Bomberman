//-----------------------------------
//main.cpp
//-----------------------------------
//
//Notes: 
//This project initially started as a C++ and DirectX Study that got much larger than i ever anticipated. With that said here are a few notes that will help when reading through the project.
//
//Every class with the exception of a few need to have Initialize called before the object can be used. This functionality is used primarily to detect if the initialization has failed (returns true || false).
//It was much easier to debug through the systems this way
//
//Every class also has a separate private methods that is called Shutdown(). This methods is called when the deconstructor is called.
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "systemclass.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pScmdline, int iShowCmd)
{
	SystemClass* system;
	bool result;

	system = new SystemClass;
	if(!system)
		return 0;
	
	//Seed Random
	srand(static_cast<unsigned int>(time(0)));
	
	result = system->Initialize();
	if(result)
		system->Run();

	delete system;
	system = 0;

	return 0;
}