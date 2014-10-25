//------------------------------------------------------
//loading.h
//------------------------------------------------------
//This class will switch between different levels during the duration of the game - this is never used in the current implementation

#ifndef _LOADING_H_
#define _LOADING_H_

#include "baselevel.h"
#include "static2D.h"

class Loading : public BaseLevel
{
public:
	Loading();
	Loading(const Loading&);
	~Loading();

	bool Initialize(shared_ptr<D3D>, HWND);
	bool Render();

private:
	//Methods
	void Shutdown();

	//Variables
	shared_ptr<Static2D> m_pImage;
	Text* m_pTextFPS;

};

#endif//_LOADING_H_