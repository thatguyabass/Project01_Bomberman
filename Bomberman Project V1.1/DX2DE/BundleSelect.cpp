//--------------------------------------------------------------------------------
//BundleSelect.cpp
//--------------------------------------------------------------------------------
//

#include "BundleSelect.h"

BundleSelect::BundleSelect()
{

}

BundleSelect::~BundleSelect()
{
	Shutdown();
}


bool BundleSelect::Initialize(ID3D11Device* device, shared_ptr<Font> font, wstring dir, function<void(int)> levelCallback, int callbackValue, function<void(void)> backCallback)
{
	bool result;
	
	result = LevelSelect::Initialize(device, font, dir, levelCallback, callbackValue, backCallback);
	if(!result)
		return false;

	return true;
}

void BundleSelect::Update(float gameTime)
{
	LevelSelect::Update(gameTime);
}

bool BundleSelect::Render(ID3D11DeviceContext* context, shared_ptr<TextureShader> shader, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho)
{
	bool result;

	result = LevelSelect::Render(context, shader, world, view, ortho);
	if(!result)
		return false;

	return true;
}

void BundleSelect::Shutdown()
{

}