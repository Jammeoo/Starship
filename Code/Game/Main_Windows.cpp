#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"

#include"Engine/Input/InputSystem.hpp"
#include"Engine/Math/Vec2.hpp"
#include"Engine/Renderer/Renderer.hpp"
//-----------------------------------------------------------------------------------------------

extern InputSystem* g_theInput;
extern App* g_theApp;


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	UNUSED( commandLineString );
	UNUSED(applicationInstanceHandle);
	g_theApp = new App();
	g_theApp->Startup();
	g_theApp->Run();

	g_theApp->Shutdown();
	delete g_theApp;
	g_theApp = nullptr;
	delete g_theInput;
	g_theInput = nullptr;
	return 0;
}


