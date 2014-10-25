//-----------------------------------------------------------
//AutoDropdown.h
//-----------------------------------------------------------
//This drop down will auto populate using text files found in the directory provided

#ifndef _AUTO_DROPDOWN_H_
#define _AUTO_DROPDOWN_H_

#include "Dropdown.h"

class AutoDropdown : public Dropdown
{
public:
	AutoDropdown();
	~AutoDropdown();

	bool Initialize(ID3D11Device*, shared_ptr<Font>, wstring, wstring, int, int, XMFLOAT2, std::function<void(int)>, int, float BOX_MOD = 1.0f, float SCALE = 1.0f);
	void Update(float, BoundingBox* = NULL);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

private:
	//Methods
	void Shutdown();

};

#endif//_AUTO_DROPDOWN_H_