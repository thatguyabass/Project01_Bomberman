//----------------------------------------------
//base2D.h
//----------------------------------------------
//Class that is handles the Loading and Rendering of the 2D textures using the Texture Shader
//This class also stores all the based vaiables that all 2D objects in the application can use/will use

#ifndef _BASE2D_H_
#define _BASE2D_H_

#include<D3D11.h>

#include<Windows.h>
#include<xnamath.h>

#include "textureclass.h"
#include "textureshader.h"
#include "collisions.h"
#include "Color.h"

#include "Util.h"
#include "stringManager.h"

//---------------------------------------------------------------------------------------------------------------------------

//Local and World Positions are used primarily by the GUI for the Hub or moving buttons
#define LOCAL m_transform.localPosition
#define POSITION m_transform.position
#define ROTATION m_transform.rotation

class Base2D
{
public:
	Base2D();
	Base2D(const Base2D&);
	virtual ~Base2D();

	virtual bool Initialize(ID3D11Device*, int imageWidth, int imageHeight, XMFLOAT2, float);
	virtual void Update(float);
	virtual bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	//Local Position and Position Access Method
	void SetPosition(XMFLOAT2);
	XMFLOAT2 GetPosition();
	void SetLocal(XMFLOAT2);
	XMFLOAT2 GetLocal();

	//Dimension Access Methods
	void SetDimensions(XMFLOAT2);
	XMFLOAT2 GetDimensions();

	//Color Access Methods
	void SetColor(XMFLOAT4);
	XMFLOAT4 GetColor();

	//Offset is used with the location position to maintain position when other elements have moved - Hud
	virtual void SetOffset(XMFLOAT2);
	XMFLOAT2 GetOffset();

	int GetIndexCount();
	//Used to render the Object
	ID3D11ShaderResourceView* GetTexture();

	//Collision Methods
	void SetBox();
	BoundingBox* GetBox();
	bool CheckIntersect(BoundingBox*);
	bool CheckIntersect(BoundingBox*, int&);

protected:
	//Variables used to communicate the values of this object to the Texture Shader
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

	//Transform of the object
	struct Transform
	{
		XMFLOAT2 localPosition;
		XMFLOAT2 position;
		float rotation;
	};

	//Methods	
	virtual void Shutdown();
	bool InitializeBuffer(ID3D11Device*);
	void ShutdownBuffer();
	//Prepare the Buffer to be rendered
	bool UpdateBuffer(ID3D11DeviceContext*);
	//Render the Buffer
	void RenderBuffer(ID3D11DeviceContext*);
	//Render the Texture
	bool RenderShader(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4& world, XMFLOAT4X4& view, XMFLOAT4X4& ortho);

	//Load the Texture
	bool LoadTexture(ID3D11Device*, wstring);
	void ReleaseTexture();

	//Initialize the Position, Rotation and Scale of the object
	void InitializeTransform(XMFLOAT2, float);

	//Update the Vertex before being sent to be rendered
	virtual void VertexUpdate(VertexType*, float left, float right, float top, float bottom);
	//Override this function to play custom animations
	virtual void UpdateTexture(VertexType*);
	//Updates the color of the Vertex
	virtual void UpdateColor(VertexType*);
	
	//Variables
	//Render
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	shared_ptr<Texture> m_pTexture;
	int m_vertexCount, m_indexCount;

	//Image Specs
	int m_imageWidth, m_imageHeight;
	int m_previousPosX, m_previousPosY;
	int m_previousRot;
	XMFLOAT4 m_previousColor;
	int m_previousFrame;

	XMFLOAT2 m_offset;
	XMFLOAT4 m_colorOverride;

	//Positional
	Transform m_transform;
	//Collision
	float m_boundsMod;
	shared_ptr<BoundingBox> m_pBox;

};

#endif//_BASE2D_H_