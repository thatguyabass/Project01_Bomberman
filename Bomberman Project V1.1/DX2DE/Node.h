//----------------------------------------------------
//Node.h
//----------------------------------------------------
//Handle the pathfinding and visual element of the node
//These nodes will be used by the Level Data class to create the level

#ifndef _NODE_H_
#define _NODE_H_

#include "BaseNode.h"

#include "text.h"
#include "font.h"

//--------------------------------------------------------------------------------------------------------

class Node : public BaseNode
{
public:
	Node();
	~Node();

	bool Initialize(ID3D11Device* device, int imageWidth, int imageHeight, char, XMFLOAT2 position, int id, XMFLOAT2 index, XMFLOAT2& spawn, XMFLOAT2& exit);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& orthoView, XMFLOAT4X4& ortho);

	bool Destroy();//Destroy a tile

	bool operator==(const Node&);
	bool operator==(Node&);

	//Pathfinding Debug
	bool InitializeText(ID3D11Device*, shared_ptr<Font>, int, int, HWND, float scale = 0.75f);
	void UpdateText(float);
	bool RenderText(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//Debug
	Static2D* GetDebugNode();

private:
	//Methods
	void Shutdown();

	//Variables
	shared_ptr<Texture> m_pAlt;
	
	shared_ptr<Text> m_pFText;
	shared_ptr<Text> m_pHText;
	shared_ptr<Text> m_pGText;

};

#endif//_TILE_H_