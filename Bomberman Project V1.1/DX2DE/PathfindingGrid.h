//---------------------------------------------
//PathfindingGrid.h
//---------------------------------------------
//Initialize, Update, and draw the Node objects

#ifndef _PATH_GRID_H_
#define _PATH_GRID_H_

#include "LData.h"

#include "Node.h"
#include "Connection.h"

#include "Color.h"
#include "Pathfinding.h"

class PathGrid
{
public:
	PathGrid();
	PathGrid(const PathGrid&);
	~PathGrid();

	bool Initialize(ID3D11Device*, int, int, int, int, float, float, LData*);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	bool InitializeText(ID3D11Device*, int, int, HWND);
	void UpdateText(float);
	bool RenderText(ID3D11DeviceContext*, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	Node* GetNode(int, int);
	void FindPath(Enemy*);
	void FindPath(Seeker*);

private:
	//Methods
	void Shutdown();

	XMFLOAT2 GetNodePosition(Node*);
	void AddNode(ID3D11Device*, int, int, Node*, Node*);
	void MakeConnection(ID3D11Device*, int, int, Node*, Node*);

	//Variables
	Node** m_ppNode;
	int m_connectionCount;
	vector<Connection*> m_pConnection;

	shared_ptr<Pathfinding> m_pPath;
	
	//DEBUG
	FontShader* m_pFontShader;
	Font* m_pFont;

	int m_X;
	int m_Y;

};

#endif//_TILE_PATH_H_