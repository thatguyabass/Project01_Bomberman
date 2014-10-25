//------------------------------------------------------------------------------------
//Connection.h
//------------------------------------------------------------------------------------
//Initialize and Render the Connection
//Debug to display the connection between two nodes

//Future development - Need to make this dynamic between any Base2D Object

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "base2D.h"
#include "Node.h"
#include "EditorTile.h"

//------------------------------------------------------------------------------------------------

class Connection : public Base2D
{
public:
	Connection();
	Connection(const Connection&);
	~Connection();

	bool Initialize(ID3D11Device*, XMFLOAT2, XMFLOAT2, XMFLOAT4);
	void Update(float);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& orthoView, XMFLOAT4X4& ortho);

	void SetRender(bool);
	bool GetRender();
	void SetObjects(Node*, Node*);//Set to render between two nodes
	void SetObjects(EditorTile*, EditorTile*);//Render between two editor tiles

private:
	//Methods
	void Shutdown();

	//Variables
	bool m_render;
	Node* m_pN1;
	Node* m_pN2;
	
	EditorTile* m_pED1;
	EditorTile* m_pED2;

};

#endif//_CONNECTION_H_