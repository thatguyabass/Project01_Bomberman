//---------------------------------------------------
//LevelEditorWaypointRender.h
//---------------------------------------------------
//Draw the selected node and the additional waypoints in numarical order

#ifndef _LEVEL_EDITOR_WAYPOINT_RENDER_H_
#define _LEVEL_EDITOR_WAYPOINT_RENDER_H_

#include "static2D.h"
#include "text.h"
#include "font.h"

#include "EditorGrid.h"

//---------------------------------------------------------------------------------------

class EdWaypointRender
{
public:
	EdWaypointRender();
	~EdWaypointRender();
	
	bool Initialize(ID3D11Device*, shared_ptr<Font>, shared_ptr<EditorGrid>);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);
	 
	//Set the Selected Node
	void SetSelected(int, int);
	//Remove the Selected Node
	void Deselect();
	bool GetRender();
	
	//Waypoint Manipulation
	void AddWaypoint(int, int);
	void AddWaypoint(XMFLOAT2);
	void RemoveWaypoint(int);
	//Clear the Waypoints
	void Clear();

private:
	//Methods
	void Shutdown();

	//Variables
	bool m_render;

	int m_textCount;
	vector<shared_ptr<Text>> m_pText;
	shared_ptr<Static2D> m_pSelected;//Visual Object to show the user which object is selected
	shared_ptr<EditorGrid> m_pGrid;

	ID3D11Device* m_pDevice;
	shared_ptr<Font> m_pFont;

};

#endif//_LEVEL_EDITOR_WAYPOINT_RENDER_H_