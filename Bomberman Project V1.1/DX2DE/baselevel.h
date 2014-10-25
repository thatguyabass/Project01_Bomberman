//--------------------------
//baselevel.h
//--------------------------
//Base Class for all the levels used in this application
//Contains the variables and methods that can be used to easily create new levels

#ifndef _BASE_LEVEL_H_
#define _BASE_LEVEL_H_

#include "d3d.h"
#include "input.h"

#include "stringManager.h"

#include "orthographiccamera.h"
#include "textureshader.h"

#include "font.h"
#include "text.h"

#include "stats.h"

#include "Util.h"
#include "fps.h"

#include<memory>
#include<vector>

//----------------------------------------------------------------------------------------------------------

class BaseLevel
{
public:
	BaseLevel();
	BaseLevel(const BaseLevel&);
	virtual ~BaseLevel();

	virtual bool Initialize(shared_ptr<D3D>);
	virtual void Update(float);
	virtual bool Render();

	void GetViewMatrix(XMFLOAT4X4&);
	XMFLOAT2 GetCameraOffset();

protected:	
	//Methods
	virtual void Shutdown();
	virtual void InitializeCamera();

	void BeginScene();//Set the Scene before rendering
	void EndScene();//Close the Scene after the rendering is completed

	//Variables
	shared_ptr<D3D> m_pD3D;

	shared_ptr<OrthoCamera> m_pCamera;
	shared_ptr<TextureShader> m_pTextureShader;

};

#endif //_BASE_LEVEL_H_