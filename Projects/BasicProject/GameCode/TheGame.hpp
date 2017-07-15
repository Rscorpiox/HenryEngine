#pragma once

#ifndef THEGAME_HPP
#define THEGAME_HPP

#include <stdio.h>
#include <Windows.h>
#include <Xinput.h>					// include the Xinput API
#pragma comment( lib, "xinput" )	// Link in the xinput.lib static library

#include "Engine\Memory\MemoryAllocatePool.hpp"
#include "Engine\Core\Camera3D.hpp"
#include "Engine\Renderer\Material.hpp"
#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"
#include "Engine\Renderer\BitmapFont.hpp"
#include "Engine\Core\Clock.hpp"


namespace Henry
{

enum GameState{ STARTING_UP = 0 , PLAYING, QUITTING };


class TheGame
{
public:
	TheGame(void);
	~TheGame(void);
	void Input();
	void Update( float deltaSeconds );
	void Render();
	GameState GetGameState();
	void SetGameState(GameState state);

public:
	bool m_isQuitting;
	
private:
	void Initialize();
	void LoadDataFiles();
	void SetUpPerspectiveProjection();
	void ApplyCameraTransform( const Camera3D& camera );
	void DrawFrameBuffer( FBO& toDraw, Material* fboMaterial );
	void RenderAxes();
	void UpdateCameraFromMouseAndKeyboard( Camera3D& camera, float deltaSeconds );
	void PollKeyStates();
	void ToggleCursorDisplay(bool show);
	Vec2f GetMouseMovementSinceLastChecked();

	void RenderPlaying();

private:
	Material* m_material;
	Camera3D m_camera;
	FBO m_fbo;
	Vec2f m_centerCursorPos;
	double m_timeOfLastUpdate;
	GameState m_gameState;
	InputSystem* m_input;
	BitmapFont* m_font;
	Clock* m_clock;
	int* m_intMemoryTest;
	std::vector<float*> m_floatMemoryTest;
};

};

#endif