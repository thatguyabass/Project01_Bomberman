//-------------------------------------------------------------
//LevelEditorEnemies.h
//-------------------------------------------------------------
//Handle the placement of waypoints for enemies

#ifndef _LEVEL_EDITOR_ENEMIES_H_
#define _LEVEL_EDITOR_ENEMIES_H_

#include "Button.h"
#include "static2D.h"

#include "font.h"
#include "text.h"

#include "EditorGrid.h"
#include "LevelEditorWaypointRender.h"

#include<vector>

//-------------------------------------------------------------------------------------------------

class LevEdEnemies
{
public:
	LevEdEnemies();
	~LevEdEnemies();

	bool Initialize(ID3D11Device*, shared_ptr<Font>, shared_ptr<EditorGrid>);
	void Update(float, XMFLOAT2);
	bool Render(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);
	//Render the Waypoint Overlay - needed to correct draw order
	bool RenderWaypointOverlay(ID3D11DeviceContext*, shared_ptr<TextureShader>, XMFLOAT4X4&, XMFLOAT4X4&, XMFLOAT4X4&);

	//State Access methods
	int GetState();
	void SetState(int);

	//Highlight the Selected Node
	void SelectedNode(int, int);

	//States
	enum WAYPOINT_STATE { NONE, SELECT, PLACE, REMOVE, CONFIRM };

private:
	//Methods
	void Shutdown();
	//Check to see if the currently selected Node is already being used
	bool FindWaypoints(int, int, int&);

	//State
	WAYPOINT_STATE e_waypoint;

	//Variables
	vector<shared_ptr<Button>> m_pButtons;
	shared_ptr<Text> m_pText;
	shared_ptr<EditorGrid> m_pGrid;
	shared_ptr<Static2D> m_pToggle;

	vector<XMFLOAT2> m_pos;
	EditorTile* m_pSelected;
	shared_ptr<EdWaypointRender> m_pWaypoint;

};
#endif//_LEVEL_EDITOR_ENEMIES_H_