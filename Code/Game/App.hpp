#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Time.hpp"
#include"Engine/Core/ErrorWarningAssert.hpp"

class App 
{
public:
	App();
	~App();
	void Startup();
	void Shutdown();
	void Run();
	void RunFrame();

	bool IsQuitting() const { return m_isQuitting; }
	void HandleKeyPressed( unsigned char keyCode );
	void HandleKeyReleased(unsigned char keyCode);
	void UpdateFromKeyBoard();
	void UpdateFromController();
	bool HandleQuitRequested();
	static bool Command_Quit(EventArgs& args);
	static bool Event_SetGameTimeScale(EventArgs& args);
private:
	void BeginFrame();
	void Update( );
	void Render() const;
	void EndFrame();
	
public:
	Game* m_theGame;
	bool   m_isQuitting = false;

	bool   m_isSlowMo = false;
	bool   m_doSwitch = false;
	Vec2   m_shipPos;

	//DevConsole
	Camera m_gameCamera;
	AABB2 m_devConsoleAABB2 = AABB2(0.f, 0.f, 0.f, 0.f);
};