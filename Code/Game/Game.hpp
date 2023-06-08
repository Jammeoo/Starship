#pragma once
#include "Game/GameCommon.hpp"
class App;
class PlayerShip;
class Asteroid;
class Bullet;
class Entity;
struct Vec2;
class Beetle;
class Debris;
class Wasp;
class Prop;
class Game 
{
public:
	Game(App* owner);
	~Game();
public:
	void Startup();
	void Update();
	void Render()const;
	void Shutdown();
	 
public:
	App*			m_app = nullptr;
	PlayerShip*		m_playerShip = nullptr;		
	Asteroid*		m_asteroids[MAX_ASTEROIDS] = { nullptr };	
	Beetle*			m_beetles[MAX_BEETLES] = { nullptr };
	Bullet*			m_bullets[MAX_BULLETS] = { nullptr };	
	Debris*			m_debris[MAX_DEBRIS] = { nullptr };
	Wasp*			m_wasps[MAX_WASP] = { nullptr };
	Prop*			m_props[MAX_PROP] = { nullptr };
	Camera			m_worldCamera;
	Camera			m_screenCamera;
	int				m_bulletNum = 0;
	int				m_asteroidNum = 0;
	float			m_time = 0.f;
	float			m_shakingTime = 0.f;
	SoundID			m_backgroundmusic;
	bool			m_timerSwitch = false;
	int				m_currentLevel = 1;
	int				m_numberOfDeadEnemy = 0;
	bool			m_isLevelSpawned = false;
	bool			m_isDebugDraw = false;
	bool			m_isAttractMode = true;
	bool			m_isShakingScreen = false;
	bool			m_isNaviMode = false;
	bool			m_isStarRendered = false;
	bool			m_isVicOrLoseSoundPlayed = false;
	Vec2			m_starsPositions[300] = {};

	Clock* m_clock = nullptr;

	Asteroid*		SpawmRandomAsteroid();
	Bullet*			SpawnBullet(Vec2 const& pos, float forwardDegrees);
	Beetle*			SpawnRandomBeetle();
	Wasp*			SpawnRandomWasp();
	Prop*			SpawnRandomProp();
	void			SpawnPlayership();
	void			SpawnDebrisCluster(int debrisNumber, Vec2 const& pos, Vec2 velocity, float maxRadius, float minRadius,Rgba8 color);//Two more parameter
	void			SpawnEntitiesBasedOnLevel(int currentLevel);
	void			ManageAudio(int currentLevel);
	void			NavigationMode(Bullet& bullet, Entity& entity);
	void			EditLevelCount(const float deltaSeconds);
	void			KillAllEnemy();
	bool			PauseForThreeSeconds(float deltaSeconds);
	void			JostleCamera(Vec2 maxTranslation); 
public:
	bool			IsEntityAlive(Entity* liveEntity);
	Vec2			SpawnOffscreenPosition();
	void			DrawAttactMode() const;
	void			DrawPlayButton() const;
	void			DrawFakePlayerShip(Vec2 const& position, float scale, float orientationDegree) const;
	void 			UpdateEntities( float deltaSeconds);
	void 			CheckBulletsVsAsteroids();
	void 			CheckBulletVsAsteroid(Bullet& bullet, Asteroid& asteroid);
	void 			CheckAsteroidsVsShips();
	void 			CheckAsteroidVsShip(Asteroid& asteroid, PlayerShip& playerShip);
	void 			CheckBeetlesVsShips();
	void 			CheckBeetleVsShip(Beetle& beetle, PlayerShip& playerShip);
	void 			CheckBulletsVsBeetles();
	void 			CheckBulletVsBeetle(Bullet& bullet, Beetle& beetle);
	void 			CheckBulletsVsWasps();
	void 			CheckBulletVsWasp(Bullet& bullet, Wasp& wasp);
	void			CheckPropsVsShips();
	void			CheckPropVsShip(Prop& prop, PlayerShip& playerShip);
	void 			CheckWaspsVsShips();
	void 			CheckWaspVsShip(Wasp& wasp, PlayerShip& playerShip);
	void 			DebugRenderEntities() const;
	void 			RenderEntities() const;
	void 			RenderUI()const;
	void 			RenderStarSky()const;
	void 			DrawStar(Vec2 const& position, float scale, float orientationDegree)const;
	void 			CheckGameEndWithPlayerDie(float deltaSeconds);
	void 			DeleteGarbageEntities();
};