#pragma once

#ifndef THEAPPLICATION_HPP 
#define THEAPPLICATION_HPP

#include "Engine\Input\InputSystem.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\DeveloperConsole.hpp"
#include "Engine\Core\Profiling.hpp"
#include "TheGame.hpp"


namespace Henry
{


class TheApplication
{
public:
	TheApplication(void* platformHandle);
	~TheApplication(void);
	void Initialize();
	void Run();

public:
	TheGame* m_theGame;
	DeveloperConsole* m_console;
	InputSystem* m_input;
	//OpenGLRenderer* m_renderer;
	Vec2i m_windowSize;
	Vec2i m_windowCenter;
	void* m_platformHandle;
	bool m_isQuitting;
	BitmapFont* m_font;

private:
	void Input();
	void Update();
	void Render();
	bool m_showConsole;
};

extern TheApplication* g_theApplication;

};

#endif