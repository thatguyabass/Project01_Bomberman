//----------------------------------------------------------------
//BundleSelect.h
//----------------------------------------------------------------
//Handles the bundle selection of the user

#ifndef _BUNDLE_SELECT_H_
#define _BUNDLE_SELECT_H_

#include "LevelSelect.h"

class BundleSelect : public LevelSelect
{
public:
	BundleSelect();
	~BundleSelect();

	//Level Callback will set the next level to be loaded when the Confirm button is selected
	bool Initialize(ID3D11Device*, shared_ptr<Font>, wstring, function<void(int)> levelCallback, int callbackValue, function<void(void)> backCallback);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

private:
	//Methods
	void Shutdown();

};

#endif//_BUNDLE_SELECT_H_