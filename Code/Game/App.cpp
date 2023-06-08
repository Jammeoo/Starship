#pragma once
#include"Game/App.hpp"
#include"Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include"Engine/Core/Vertex_PCU.hpp"
#define UNUSED(x) (void)(x);
class Game;
												
Renderer*					g_theRenderer = nullptr;
InputSystem*				g_theInput = nullptr;
AudioSystem*				g_theAudio = nullptr;
App*						g_theApp = nullptr;
Window*						g_theWindow = nullptr;
RandomNumberGenerator*		g_rng = nullptr;
Vec2 DEFAULT_GAME_SCREEN_CAMERA_ORTHO_BOTTOMLEFT = Vec2(0.f, 0.f);
Vec2 GAME_SCREEN_CAMERA_ORTHO_TOPRIGHT = Vec2(1600.f, 800.f);
	App::App() 
	{
		m_shipPos = Vec2(1.f, 50.f);
		m_devConsoleAABB2 = AABB2(Vec2(0.f, 0.f), GAME_SCREEN_CAMERA_ORTHO_TOPRIGHT);
	}
	App::~App() 
	{

	}
void App::Startup()
{
	g_eventSystem = new EventSystem();

	InputSystemConfig inputConfig;
	g_theInput = new InputSystem( inputConfig );	

	WindowConfig windowConfig;
	windowConfig.m_windowTitle = "C32 Starship Gold(refactored)";
	windowConfig.m_clientAspect = 2.0f;
	windowConfig.m_inputSystem = g_theInput;
	g_theWindow = new Window(windowConfig);


	RendererConfig renderConfig;
	renderConfig.m_window = g_theWindow;
	g_theRenderer = new Renderer(renderConfig);


	DevConsoleConfig devConsoleConfig;
	m_gameCamera.m_orthographicBottomLeft = DEFAULT_GAME_SCREEN_CAMERA_ORTHO_BOTTOMLEFT;
	m_gameCamera.m_orthographicTopRight = GAME_SCREEN_CAMERA_ORTHO_TOPRIGHT;

	devConsoleConfig.m_camera = &m_gameCamera;
	devConsoleConfig.m_renderer = g_theRenderer;
	g_theDevConsole = new DevConsole(devConsoleConfig);

	AudioSystemConfig audioConfig;
	g_theAudio = new AudioSystem( audioConfig );
	g_eventSystem->Startup();
	g_theDevConsole->Startup();

	g_theInput->Startup();
	g_theWindow->Startup();
	g_theRenderer->Startup();
	g_theAudio->Startup();

	g_rng = new RandomNumberGenerator();

	m_theGame = new Game(this);
	m_theGame->Startup();
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "CONTROLS");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "P - Pause");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "O - SingleStep");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "ESC - Pause/Quit");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "F1 - Debug View");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "ESDF - To Move");
	g_eventSystem->SubscribeEventCallbackFunction("quit", Command_Quit);
	g_eventSystem->SubscribeEventCallbackFunction("setgametimescale", Event_SetGameTimeScale);
	m_theGame->m_clock = new Clock(Clock::GetSystemClock());
}

void App::Shutdown()
{
	g_theDevConsole->Shutdown();
	g_theInput->Shutdown();
	g_theWindow->Shutdowm();
	g_theRenderer->Shutdown();
	g_theAudio->Shutdown();
	m_theGame->Shutdown();
	delete m_theGame;
	delete g_theRenderer;
	delete g_rng;
	delete g_theInput;
	delete g_theAudio;
	g_theAudio = nullptr;
	g_theRenderer = nullptr;
	g_theInput = nullptr;
	g_rng = nullptr;
	m_theGame = nullptr;
}

void App::Run()
{
	// Program main loop; keep running frames until it's time to quit
	while (!IsQuitting())
	{
		RunFrame();
	}
}

void App::BeginFrame()
{

	g_theDevConsole->BeginFrame();
	g_theInput->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
	g_theAudio->BeginFrame();

}


void App::Render() const
{
	g_theRenderer->ClearScreen(Rgba8(0, 0, 0, 255));
	g_theRenderer->SetModelConstants();
	g_theRenderer->SetDepthMode(DepthMode::DISABLED);
	g_theRenderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	g_theRenderer->BindShader(nullptr);
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->BeginCamera(Camera());
	m_theGame->Render();
	g_theRenderer->EndCamera(Camera());

	g_theRenderer->BeginCamera(m_gameCamera);
	g_theDevConsole->Render(m_devConsoleAABB2);
	g_theRenderer->EndCamera(m_gameCamera);
}
void App::Update()
{
	m_gameCamera.SetOrthoView(DEFAULT_GAME_SCREEN_CAMERA_ORTHO_BOTTOMLEFT, GAME_SCREEN_CAMERA_ORTHO_TOPRIGHT);
	m_theGame->Update();
	UpdateFromKeyBoard();
	UpdateFromController();
}
void App:: EndFrame()
{
	g_theInput->EndFrame();
	g_theAudio->EndFrame();
	g_theWindow->EndFrame();
	g_theRenderer->EndFrame();
}

void App::RunFrame()
{
	Clock::TickSystemClock();
	BeginFrame();			
	Update();
	Render();				
	EndFrame();				
}

void App::HandleKeyPressed(unsigned char keyCode) 
{
	g_theInput->HandleKeyPressed(keyCode);
}

bool App::Command_Quit(EventArgs& args)
{
	UNUSED(args);
	g_theApp->HandleQuitRequested();
	return false;
}

bool App::Event_SetGameTimeScale(EventArgs& args)
{
	float timeScale = args.GetValue("scale", -1.f);
	if (timeScale == -1.f)
	{
		g_theDevConsole->AddLine(DevConsole::ERROR_, "Invalid format: setgametimescale. Example:");
		g_theDevConsole->AddLine(DevConsole::WARNING, "setgametimescale scale=1.0");
		return true;
	}

	g_theApp->m_theGame->m_clock->SetTimeScale(timeScale);

	return true;

}

void App::HandleKeyReleased(unsigned char keyCode) 
{

	g_theInput->HandleKeyReleased(keyCode);
	//DebuggerPrintf("Key #%i was released\n", keyCode);
}
bool App::HandleQuitRequested() 
{
	m_isQuitting = true;
	return true;
}

void App::UpdateFromKeyBoard()
{
	if (g_theInput->WasKeyJustPressed(' ')|| g_theInput->WasKeyJustPressed('N'))
	{
		if (m_theGame->m_isAttractMode) 
		{
			m_theGame->SpawnPlayership();
			m_theGame->m_isAttractMode = false;
		}
	}
	if (g_theInput->WasKeyJustPressed('K')) 
	{
		m_theGame->KillAllEnemy();
	}
	if (g_theInput->WasKeyJustPressed('P'))
	{
		m_theGame->m_clock->TogglePause();
	}
	if (g_theInput->WasKeyJustPressed('O'))
	{
		m_theGame->m_clock->StepSingleFrame();
	}
	if (g_theInput->WasKeyJustPressed('T'))
	{
		m_isSlowMo = !m_isSlowMo;
		if (m_isSlowMo) 
		{
			m_theGame->m_clock->SetTimeScale(0.1f);
		}
		else
		{
			m_theGame->m_clock->SetTimeScale(1.f);
		}
	}
	if (g_theInput->WasKeyJustPressed(119))
	{
		g_theAudio->StopSound(m_theGame->m_backgroundmusic);
		Shutdown();
		Startup();
	}
	if (g_theInput->WasKeyJustPressed(27))
	{
		if (!m_theGame->m_isAttractMode)
		{
			g_theAudio->StopSound(m_theGame->m_backgroundmusic);
			m_theGame->Shutdown();
			m_theGame->Startup();
			m_theGame->m_isAttractMode = true;
		}
		else
		{
			HandleQuitRequested();
		}
		
	}

}

void App::UpdateFromController()
{
	XboxController const& controller = g_theInput->GetController(0);
	//Respawn
	if (controller.WasButtonJustPressed(XboxButtonID::XBOX_BUTTON_A)) {
		if (m_theGame->m_isAttractMode) 
		{
			m_theGame->SpawnPlayership();
		}
		m_theGame->m_isAttractMode = false;
	}
	//Exit
	if (controller.WasButtonJustReleased(XboxButtonID::XBOX_BUTTON_BACK)) {
		if (!m_theGame->m_isAttractMode)
		{
			Shutdown();
			Startup();
			m_theGame->m_isAttractMode = true;
		}
		else
		{
			HandleQuitRequested();
		}
	}
}
