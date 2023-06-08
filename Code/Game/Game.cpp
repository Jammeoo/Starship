#include"Game/Game.hpp"
#include"Game/App.hpp"
#include"Game/PlayerShip.hpp"
#include"Game/Asteroid.hpp"
#include"Game/Bullet.hpp"
#include"Game/Beetle.hpp"
#include"Game/Wasp.hpp"
#include"Game/Debris.hpp"
#include "Game/Prop.hpp"
#include"Engine/Renderer/Camera.hpp"
#include"Engine/Math/MathUtils.hpp"
#include"Engine/Core/ErrorWarningAssert.hpp"
#include"Engine/Core/StringUtils.hpp"

Vec2 DEFAULT_CAMERA_ORTHO_BOTTOMLEFT = Vec2(0.f, 0.f);
Vec2 WORLD_CAMERA_ORTHO_TOPRIGHT = Vec2(200.f, 100.f);
Vec2 SCREEN_CAMERA_ORTHO_TOPRIGHT = Vec2(1600.f, 800.f);

Game::Game(App* owner)
{
	UNUSED(owner);
	m_worldCamera.m_orthographicBottomLeft = DEFAULT_CAMERA_ORTHO_BOTTOMLEFT;
	m_worldCamera.m_orthographicTopRight = WORLD_CAMERA_ORTHO_TOPRIGHT;
	m_screenCamera.m_orthographicBottomLeft = DEFAULT_CAMERA_ORTHO_BOTTOMLEFT;
	m_screenCamera.m_orthographicTopRight = SCREEN_CAMERA_ORTHO_TOPRIGHT;
	for (int i = 0; i < 300; i++) 
	{
		float x = g_rng->RollRandomFloatInRange(0.f, WORLD_SIZE_X);
		float y = g_rng->RollRandomFloatInRange(0.f, WORLD_SIZE_Y);
		m_starsPositions[i] = Vec2(x, y);
	}
}

Game::~Game()
{

}

void Game::Startup()
{
	const Vec2 worldCenter(WORLD_SIZE_X * 0.5f, WORLD_SIZE_Y * 0.5f);
	m_worldCamera.SetOrthoView(DEFAULT_CAMERA_ORTHO_BOTTOMLEFT, WORLD_CAMERA_ORTHO_TOPRIGHT);
	m_screenCamera.SetOrthoView(DEFAULT_CAMERA_ORTHO_BOTTOMLEFT, SCREEN_CAMERA_ORTHO_TOPRIGHT);
	if (m_isAttractMode) 
		return;
}

void Game::Update()
{
	float deltaSeconds = m_clock->GetDeltaSeconds();

	m_worldCamera.SetOrthoView(DEFAULT_CAMERA_ORTHO_BOTTOMLEFT, WORLD_CAMERA_ORTHO_TOPRIGHT);
	m_screenCamera.SetOrthoView(DEFAULT_CAMERA_ORTHO_BOTTOMLEFT, SCREEN_CAMERA_ORTHO_TOPRIGHT);
	if (m_isAttractMode) 
	{
		return;
	}
	if (!m_isLevelSpawned)
	{
		SpawnEntitiesBasedOnLevel(m_currentLevel);
	}
	EditLevelCount(deltaSeconds);
	
	UpdateEntities(deltaSeconds);
	CheckBulletsVsAsteroids();
	CheckAsteroidsVsShips();
	
	CheckBeetlesVsShips();
	CheckBulletsVsBeetles();
	CheckPropsVsShips();
	CheckWaspsVsShips();
	CheckBulletsVsWasps();

	CheckGameEndWithPlayerDie(deltaSeconds);
	DeleteGarbageEntities();
	if (m_isShakingScreen) 
	{
		m_shakingTime += deltaSeconds;
		JostleCamera(Vec2(1.f, 2.f));
	}
}

void Game::Render() const
{

	g_theRenderer->BeginCamera(m_screenCamera);
	if (m_isAttractMode) 
	{
		DrawAttactMode();
	}
	g_theRenderer->EndCamera(m_screenCamera);

	if (m_isAttractMode) 
		return;
	g_theRenderer->BeginCamera(m_screenCamera);
	RenderUI();
	g_theRenderer->EndCamera(m_screenCamera);

	g_theRenderer->BeginCamera(m_worldCamera);
	if (!m_isStarRendered)
	{
		RenderStarSky();
	}
	RenderEntities();
	m_playerShip->Render();

	if (m_isDebugDraw) 
	{
		DebugRenderEntities();
	}
	g_theRenderer->EndCamera(m_worldCamera);
}

void Game::Shutdown()
{
	if (m_isAttractMode) 
		return;
	if (m_playerShip) {
		delete m_playerShip;
		m_playerShip = nullptr;
	}
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) {
		Asteroid*& asteroid = m_asteroids[asteroidIndex];
		if (asteroid) {
			delete asteroid;
			asteroid = nullptr;
		}
	}
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++) {
		Bullet*& bullet = m_bullets[bulletIndex];
		if (bullet) {
			delete bullet;
			bullet = nullptr;
		}
	}
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++) {
		Beetle*& beetle = m_beetles[beetleIndex];
		if (beetle) {
			delete beetle;
			beetle = nullptr;
		}
	}
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++) {
		Wasp*& wasp = m_wasps[waspIndex];
		if (wasp) {
			delete wasp;
			wasp = nullptr;
		}
	}
	Prop*& prop = m_props[0];
	if (prop) {
		delete prop;
		prop = nullptr;
	}
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++) {
		Debris*& debris = m_debris[debrisIndex];
		if (debris) {
			delete debris;
			debris = nullptr;
		}
	}
	
}

Asteroid* Game::SpawmRandomAsteroid()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) 
	{
		Asteroid*& asteroid = m_asteroids[asteroidIndex];
		if (!asteroid) 
		{
			asteroid = new Asteroid(this, SpawnOffscreenPosition());
			asteroid->m_orientationDegrees = g_rng->RollRandomFloatInRange(0.f, 360.f);
			asteroid->m_angularVelocity = g_rng->RollRandomFloatInRange(-300.f, 300.f);
			float driftAngleDegrees = g_rng->RollRandomFloatInRange(0.f, 360.f);
			asteroid->m_velocity.x = ASTEROID_SPEED * CosDegrees(driftAngleDegrees);
			asteroid->m_velocity.y = ASTEROID_SPEED * SinDegrees(driftAngleDegrees);
			return asteroid;
		}
	}
	return nullptr;
}

Bullet* Game::SpawnBullet(Vec2 const& pos, float forwardDegrees)
{
	if (m_playerShip->m_isDead) return nullptr;
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex) 
	{
		Bullet*& bullet = m_bullets[bulletIndex];
		if (!bullet) 
		{
			bullet = new Bullet(this, pos);
			bullet->m_orientationDegrees = forwardDegrees;
			bullet->m_velocity.x = BULLET_SPEED * CosDegrees(forwardDegrees);
			bullet->m_velocity.y = BULLET_SPEED * SinDegrees(forwardDegrees);
			return bullet;
		}
	}
	return nullptr;
	//ERROR_RECOVERABLE("Cannot spawn a new bullet; all slots are full!");
}

void Game::NavigationMode(Bullet& bullet,Entity& entity)
{
	bool checkIsNear = DoDiscsOverlap(bullet.m_position, bullet.m_naviRadius, entity.m_position, entity.m_physicsRadius);
	Vec2 normalizedDisplace = (entity.m_position - bullet.m_position).GetNormalized();
	float theta = Atan2Degrees(normalizedDisplace.y, normalizedDisplace.x);
	float magnitude = bullet.m_velocity.GetLength();
	if (checkIsNear)
	{
		bullet.m_orientationDegrees = theta;
		bullet.m_velocity = magnitude * Vec2(CosDegrees(theta), SinDegrees(theta));
	}
}

Beetle* Game::SpawnRandomBeetle()
{
	if (m_playerShip->m_isDead) return nullptr;
	
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; ++beetleIndex) 
	{
		Beetle*& beetle = m_beetles[beetleIndex];
		if (!beetle)
		{
			beetle = new Beetle(this, SpawnOffscreenPosition());
			return beetle;
		}
	}
	return nullptr;
}

Wasp* Game::SpawnRandomWasp()
{
	if (m_playerShip->m_isDead) return nullptr;

	for (int waspIndex = 0; waspIndex < MAX_WASP; ++waspIndex) 
	{
		Wasp*& wasp = m_wasps[waspIndex];
		if (!wasp)
		{
			wasp = new Wasp(this, SpawnOffscreenPosition());
			return wasp;
		}
	}
	return nullptr;
}

Prop* Game::SpawnRandomProp()
{
	if (m_playerShip->m_isDead) return nullptr;
	Prop*& prop = m_props[0];
	if (!prop)
	{
		prop = new Prop(this, SpawnOffscreenPosition());
		return prop;
	}
	return nullptr;
}

void Game::SpawnPlayership()
{
	m_playerShip = new PlayerShip(this, Vec2(WORLD_CENTER_X, WORLD_CENTER_Y));
	SpawnEntitiesBasedOnLevel(m_currentLevel);
}

bool Game::IsEntityAlive(Entity* liveEntity)
{
	return !liveEntity->m_isDead;
}

Vec2 Game::SpawnOffscreenPosition()
{
	float x = g_rng->RollRandomFloatInRange(0.f, 0.4f * WORLD_SIZE_X);
	if (x > 0.2f * WORLD_SIZE_X) 
	{
		x = x + WORLD_SIZE_X;
	}
	else x = 0.f - x;
	float y = g_rng->RollRandomFloatInRange(0.f, 0.4f * WORLD_SIZE_Y);
	if (y > 0.2f * WORLD_SIZE_Y) 
	{
		y = y + WORLD_SIZE_Y;
	}
	else y = 0.f - y;
	return Vec2(x, y);
}

void Game::DrawAttactMode()const
{
	DrawPlayButton();
	Vec2 animaOffset = Vec2(100.f * CosDegrees(200.f * static_cast<float> (GetCurrentTimeSeconds())), 100.f * SinDegrees(200.f * static_cast<float> (GetCurrentTimeSeconds())));
	DrawFakePlayerShip(Vec2(WORLD_CENTER_X + 30.f, WORLD_CENTER_Y) * 8.f + animaOffset, 3.f, 0.f);
	DrawFakePlayerShip(Vec2(WORLD_CENTER_X - 30.f, WORLD_CENTER_Y) * 8.f + animaOffset, 3.f, 0.f);

}

void Game::DrawPlayButton()const
{
	int logoAlpha = 255;
	float vertsScaler = 0.98f + 0.02f * SinDegrees(500.f * static_cast<float> (GetCurrentTimeSeconds()));
	float logoScalar = fabsf((50.f * SinDegrees(static_cast<float> (GetCurrentTimeSeconds()))));
	Vertex_PCU verts[3];
	verts[0].m_position = Vec3(WORLD_CENTER_X + 12.5f, WORLD_CENTER_Y, 0.f) * vertsScaler * 8.f ;
	verts[1].m_position = Vec3(WORLD_CENTER_X - 7.5f, WORLD_CENTER_Y + 10.f, 0.f) * vertsScaler * 8.f;
	verts[2].m_position = Vec3(WORLD_CENTER_X - 7.5f, WORLD_CENTER_Y - 10.f, 0.f) * vertsScaler * 8.f;

	verts[0].m_color = Rgba8(50, 150, 70);
	verts[1].m_color = Rgba8(50, 150, 70);
	verts[2].m_color = Rgba8(50, 150, 70);
	verts[0].m_color.a = static_cast<unsigned char>(logoScalar * logoAlpha);
	verts[1].m_color.a = static_cast<unsigned char>(logoScalar * logoAlpha);
	verts[2].m_color.a = static_cast<unsigned char>(logoScalar * logoAlpha);
	g_theRenderer->DrawVertexArray(3, verts);
}

void Game::DrawFakePlayerShip(Vec2 const& position, float scale, float orientationDegree)const
{
	Vertex_PCU shipVerts[NUM_SHIP_VERTS-3];
	m_playerShip->InitializeLocalVerts(15, shipVerts);
	TransformVertexArrayXY3D(NUM_SHIP_VERTS - 3, shipVerts, scale, orientationDegree, position);
	g_theRenderer->DrawVertexArray(NUM_SHIP_VERTS - 3, shipVerts);
}

void Game::UpdateEntities(float deltaSeconds)
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		Bullet*& bullet = m_bullets[bulletIndex];
		if (bullet)
		{
			bullet->Update(deltaSeconds);
		}
	}
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++)
	{
		Beetle*& beetle = m_beetles[beetleIndex];
		if (beetle)
		{
			beetle->Update(deltaSeconds);
		}
	}
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++)
	{
		Wasp*& wasp = m_wasps[waspIndex];
		if (wasp)
		{
			wasp->Update(deltaSeconds);
		}
	}
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) 
	{
		Asteroid*& asteroid = m_asteroids[asteroidIndex];
		if (asteroid) 
		{
			asteroid->Update(deltaSeconds);
		}
	}
	Prop*& prop = m_props[0];
	if (prop)
	{
		prop->Update(deltaSeconds);
	}
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++) 
	{
		Debris*& debris = m_debris[debrisIndex];
		if (debris) 
		{
			debris->Update(deltaSeconds);
		}
	}

	PlayerShip*& playerShip = m_playerShip;
	playerShip->Update(deltaSeconds);
}

void Game::SpawnDebrisCluster(int debrisNumber, Vec2 const& pos, Vec2 velocity, float maxRadius, float minRadius, Rgba8 color)
{
	for (int debrisIndexAll = 0; debrisIndexAll < debrisNumber; debrisIndexAll++) 
	{
		for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; ++debrisIndex) 
		{
			Debris*& debris = m_debris[debrisIndex];
			if (!debris)
			{
				float x = g_rng->RollRandomFloatInRange(velocity.x - 2, velocity.x + 2);
				float y = g_rng->RollRandomFloatInRange(velocity.y - 2, velocity.y + 2);
				Vec2 debrisVelocity = Vec2(x, y);
				debris = new Debris(this, pos, debrisVelocity, maxRadius, minRadius, color);
				break;
			}
		}
	}
}

void Game::SpawnEntitiesBasedOnLevel(int currentPlayerLevel)
{
	switch (currentPlayerLevel)
	{
	case 1:
		ManageAudio(currentPlayerLevel);
		for (int i = 0; i < currentPlayerLevel * 4; i++) 
		{
			SpawmRandomAsteroid();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomBeetle();
		}
		
		break;
	case 2:
		ManageAudio(currentPlayerLevel);
		for (int i = 0; i < currentPlayerLevel * 4; i++) 
		{
			SpawmRandomAsteroid();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomBeetle();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomWasp();
		}
		SpawnRandomProp();
		break;
	case 3:
		
		for (int i = 0; i < currentPlayerLevel * 4; i++) 
		{
			SpawmRandomAsteroid();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomBeetle();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomWasp();
		}
		break;
	case 4:
		ManageAudio(currentPlayerLevel);
		for (int i = 0; i < currentPlayerLevel * 4; i++) 
		{
			SpawmRandomAsteroid();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomBeetle();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomWasp();
		}
		break;
	case 5:
		for (int i = 0; i < currentPlayerLevel * 4; i++) 
		{
			SpawmRandomAsteroid();
		}
		for (int i = 0; i < currentPlayerLevel; i++) 
		{
			SpawnRandomBeetle();
		}
		for (int i = 0; i < m_currentLevel; i++) 
		{
			SpawnRandomWasp();
		}
		break;
	case 6:
		ManageAudio(currentPlayerLevel);
		break;

	}
	m_isLevelSpawned = true;
}

void Game::ManageAudio(int currentlevel)
{
	SoundID backgroundMusic1 = g_theAudio->CreateOrGetSound("Data/Audio/StartLevelBgm.mp3");
	SoundID backgroundMusic2 = g_theAudio->CreateOrGetSound("Data/Audio/MiddleLevelBgm.mp3");
	SoundID backgroundMusic3 = g_theAudio->CreateOrGetSound("Data/Audio/EndLevelBgm.mp3");
	switch (currentlevel)
	{
	case 1:
		g_theAudio->StopSound(m_backgroundmusic);
		m_backgroundmusic = g_theAudio->StartSound(backgroundMusic1, true, 0.5f);
		break;
	case 2:
		g_theAudio->StopSound(m_backgroundmusic);
		m_backgroundmusic = g_theAudio->StartSound(backgroundMusic2, true, 0.5f);
		break;
	case 4:
 		g_theAudio->StopSound(m_backgroundmusic);
		m_backgroundmusic = g_theAudio->StartSound(backgroundMusic3, true, 0.5f);
		break;
	}
}

void Game::EditLevelCount(const float deltaSeconds)
{
	if (m_numberOfDeadEnemy >=5 && m_currentLevel == 1) 
	{
		m_currentLevel = 2;
		m_isLevelSpawned = false;
	}
	else if (m_numberOfDeadEnemy >= 17 && m_currentLevel == 2) 
	{
		m_currentLevel = 3;
		m_isLevelSpawned = false;
	}else if (m_numberOfDeadEnemy >= 35 && m_currentLevel == 3)
	{
		m_currentLevel = 4;
		m_isLevelSpawned = false;
	}
	else if (m_numberOfDeadEnemy >= 59 && m_currentLevel == 4) 
	{
		m_currentLevel = 5;
		m_isLevelSpawned = false;
	}
	else if (m_numberOfDeadEnemy >= 89 && (m_currentLevel == 5||m_currentLevel == 6)) 
	{
		m_currentLevel = 6;
		if (!m_isVicOrLoseSoundPlayed)
		{
			g_theAudio->StopSound(m_backgroundmusic);
			SoundID victorySound = g_theAudio->CreateOrGetSound("Data/Audio/victory.wav");
			g_theAudio->StartSound(victorySound, false, 0.5f);
			m_isVicOrLoseSoundPlayed = true;
		}
		m_timerSwitch = true;
		if (PauseForThreeSeconds(deltaSeconds))
		{
			m_isAttractMode = true;
			m_timerSwitch = false;
		}
	}
	else m_isLevelSpawned = true;
}

void Game::KillAllEnemy()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) 
	{
		Asteroid*& asteroid = m_asteroids[asteroidIndex];
		if (asteroid)
		{
			asteroid->Die();
		}
	}
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; ++beetleIndex) 
	{
		Beetle*& beetle = m_beetles[beetleIndex];
		if (beetle)
		{
			beetle->Die();
		}
	}
	for (int waspIndex = 0; waspIndex < MAX_WASP; ++waspIndex) 
	{
		Wasp*& wasp = m_wasps[waspIndex];
		if (wasp)
		{
			wasp->Die();
		}
	}
	SoundID killAllSound = g_theAudio->CreateOrGetSound("Data/Audio/lightsaber.mp3");
	g_theAudio->StartSound(killAllSound, false, 2.f);
}

bool Game::PauseForThreeSeconds(float deltaSeconds)
{
	if (m_timerSwitch) 
	{
		m_time += deltaSeconds;
	}
	else 
	{
		m_time = 0.f;
	}
	if (m_time >= 3.0f)
	{
		return true;
	}
	return false;
}

void Game::JostleCamera(Vec2 maxTranslation)
{
	if (m_isShakingScreen && m_shakingTime <= 2.f ) 
	{
		float x = g_rng->RollRandomFloatInRange(- maxTranslation.x, maxTranslation.x);
		float y = g_rng->RollRandomFloatInRange(-maxTranslation.y, maxTranslation.y);
		Vec2 tmp_ShakeDisplacement = Vec2(x, y) * (1.0f / (1.0f+ m_shakingTime));
		m_worldCamera.Translate2D(tmp_ShakeDisplacement);
	}
	if (m_shakingTime > 2.f) 
	{
		m_isShakingScreen = false;
	}
}

void Game::CheckBulletsVsAsteroids()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) 
	{
		if (m_asteroids[asteroidIndex] && IsEntityAlive(m_asteroids[asteroidIndex]))
		{
			for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
			{
				if (m_bullets[bulletIndex] && IsEntityAlive(m_bullets[bulletIndex]))
					CheckBulletVsAsteroid(*m_bullets[bulletIndex], *m_asteroids[asteroidIndex]);
			}
		}
	}
}

void Game::CheckBulletVsAsteroid(Bullet& bullet, Asteroid& asteroid)
{
	if (m_isNaviMode) NavigationMode(bullet, asteroid);
	bool checkIsCollided = DoDiscsOverlap(bullet.m_position, bullet.m_physicsRadius, asteroid.m_position, asteroid.m_physicsRadius);
	if (checkIsCollided)
	{
		bullet.m_health--;
		asteroid.m_health--;
		//DebuggerPrintf("Bullet %p hit Asteroid %p. HP is now bullet=%i", &bullet, &asteroid, bullet.m_health, asteroid.m_health);
	}
}

void Game::CheckAsteroidsVsShips()
{
	if (m_playerShip->m_isDead) 
		return;
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) 
	{
		if (m_asteroids[asteroidIndex]&& IsEntityAlive(m_asteroids[asteroidIndex]))
		{
			CheckAsteroidVsShip(*m_asteroids[asteroidIndex], *m_playerShip);
		}
	}
}

void Game::CheckAsteroidVsShip(Asteroid& asteroid, PlayerShip& playerShip)
{
	bool checkIsCollided = DoDiscsOverlap(asteroid.m_position, asteroid.m_physicsRadius, playerShip.m_position, playerShip.m_physicsRadius);
	if (checkIsCollided)
	{
		playerShip.m_health--;
		asteroid.m_health--;
	}
}

void Game::CheckBeetlesVsShips()
{
	if (m_playerShip->m_isDead) 
		return;
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++) 
	{
		if (m_beetles[beetleIndex] && IsEntityAlive(m_beetles[beetleIndex]))
		{
			CheckBeetleVsShip(*m_beetles[beetleIndex], *m_playerShip);
		}
	}
}

void Game::CheckBeetleVsShip(Beetle& beetle, PlayerShip& playerShip)
{
	bool checkIsCollided = DoDiscsOverlap(beetle.m_position, beetle.m_physicsRadius, playerShip.m_position, playerShip.m_physicsRadius);
	if (checkIsCollided)
	{
		playerShip.m_health--;
		beetle.m_health--;
	}
}

void Game::CheckBulletsVsBeetles()
{
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++) 
	{
		if (m_beetles[beetleIndex] && IsEntityAlive(m_beetles[beetleIndex]))
		{
			for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
			{
				if (m_bullets[bulletIndex] && IsEntityAlive(m_bullets[bulletIndex]))
					CheckBulletVsBeetle(*m_bullets[bulletIndex], *m_beetles[beetleIndex]);
			}
		}
	}
}

void Game::CheckBulletVsBeetle(Bullet& bullet, Beetle& beetle)
{
	if (m_isNaviMode) NavigationMode(bullet, beetle);
	bool checkIsCollided = DoDiscsOverlap(bullet.m_position, bullet.m_physicsRadius, beetle.m_position, beetle.m_physicsRadius);
	if (checkIsCollided)
	{
		bullet.m_health--;
		beetle.m_health--;
		//DebuggerPrintf("Bullet %p hit Asteroid %p. HP is now bullet=%i", &bullet, &asteroid, bullet.m_health, asteroid.m_health);
	}
}

void Game::CheckBulletsVsWasps()
{
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++) 
	{
		if (m_wasps[waspIndex] && IsEntityAlive(m_wasps[waspIndex]))
		{
			for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
			{
				if (m_bullets[bulletIndex] && IsEntityAlive(m_bullets[bulletIndex]))
					CheckBulletVsWasp(*m_bullets[bulletIndex], *m_wasps[waspIndex]);
			}
		}
	}
}

void Game::CheckBulletVsWasp(Bullet& bullet, Wasp& wasp)
{
	if (m_isNaviMode) NavigationMode(bullet, wasp);
	bool checkIsCollided = DoDiscsOverlap(bullet.m_position, bullet.m_physicsRadius, wasp.m_position, wasp.m_physicsRadius);
	if (checkIsCollided)
	{
		bullet.m_health--;
		wasp.m_health--;
		//DebuggerPrintf("Bullet %p hit Asteroid %p. HP is now bullet=%i", &bullet, &wasp, bullet.m_health, wasp.m_health);
	}
}

void Game::CheckPropsVsShips()
{
	if (m_playerShip->m_isDead)
		return;

		if (m_props[0] && IsEntityAlive(m_props[0]))
		{
			CheckPropVsShip(*m_props[0], *m_playerShip);
		}

}

void Game::CheckPropVsShip(Prop& prop, PlayerShip& playerShip)
{
	bool checkIsCollided = DoDiscsOverlap(prop.m_position, prop.m_physicsRadius, playerShip.m_position, playerShip.m_physicsRadius);
	if (checkIsCollided)
	{
		m_isNaviMode = true;
		prop.m_health--;
	}
}

void Game::CheckWaspsVsShips()
{
	if (m_playerShip->m_isDead) 
		return;
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++) 
	{
		if (m_wasps[waspIndex] && IsEntityAlive(m_wasps[waspIndex]))
		{
			CheckWaspVsShip(*m_wasps[waspIndex], *m_playerShip);
		}
	}
}

void Game::CheckWaspVsShip(Wasp& wasp, PlayerShip& playerShip)
{
	bool checkIsCollided = DoDiscsOverlap(wasp.m_position, wasp.m_physicsRadius, playerShip.m_position, playerShip.m_physicsRadius);
	if (checkIsCollided)
	{
		playerShip.m_health--;
		wasp.m_health--;
	}
}

void Game::DebugRenderEntities() const
{
	PlayerShip const* playerShip = m_playerShip;
	playerShip->DebugRender();
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++) 
	{
		Bullet const* bullet = m_bullets[bulletIndex];
		if (bullet) 
		{
			bullet->DebugRender();
			DebugDrawLine(playerShip->m_position, bullet->m_position, 0.2f, Rgba8(50, 50, 50));
		}
	}
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++) 
	{
		Asteroid const* asteroid = m_asteroids[asteroidIndex];
		if (asteroid) 
		{
			asteroid->DebugRender();
			DebugDrawLine(playerShip->m_position, asteroid->m_position, 0.2f, Rgba8(50, 50, 50));
		}
	}
	for (int beetleIndex = 0; beetleIndex < MAX_BEETLES; beetleIndex++) 
	{
		Beetle const* beetle = m_beetles[beetleIndex];
		if (beetle) 
		{
			beetle->DebugRender();
			DebugDrawLine(playerShip->m_position, beetle->m_position, 0.2f, Rgba8(50, 50, 50));
		}
	}
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++) 
	{
		Wasp const* wasp = m_wasps[waspIndex];
		if (wasp) 
		{
			wasp->DebugRender();
			DebugDrawLine(playerShip->m_position, wasp->m_position, 0.2f, Rgba8(50, 50, 50));
		}
	}
	Prop const* prop = m_props[0];
	if (prop)
	{
		prop->DebugRender();
		DebugDrawLine(playerShip->m_position, prop->m_position, 0.2f, Rgba8(50, 50, 50));
	}
	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++) 
	{
		Debris const* debris = m_debris[debrisIndex];
		if (debris) 
		{
			debris->DebugRender();
			DebugDrawLine(playerShip->m_position, debris->m_position, 0.2f, Rgba8(50, 50, 50));
		}
	}
}

void Game::RenderEntities() const
{
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
	{
		Bullet const* bullet = m_bullets[bulletIndex];
		if (bullet) bullet->Render();
	}
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		Asteroid const* asteroid = m_asteroids[asteroidIndex];
		if (asteroid) asteroid->Render();
	}
	for (int beeltleIndex = 0; beeltleIndex < MAX_BEETLES; beeltleIndex++) 
	{
		Beetle const* beetle = m_beetles[beeltleIndex];
		if (beetle) beetle->Render();
	}
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++) 
	{
		Wasp const* wasp = m_wasps[waspIndex];
		if (wasp) wasp->Render();
	}

	Prop const* prop = m_props[0];
	if (prop) prop->Render();

	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++) 
	{
		Debris const* debris = m_debris[debrisIndex];
		if (debris) debris->Render();
	}
}

void Game::RenderUI() const
{
	constexpr float padding = 64.f;
	constexpr float stridex = 64.f;
	for (int i = 0; i < m_playerShip->GetNumExtraLives(); ++i) 
	{
		float x = padding + stridex * static_cast<float>(i);
		float y = 8.f * WORLD_SIZE_Y - padding;
		DrawFakePlayerShip(Vec2(x, y), 1.f, 90.f);
	}
}

void Game::RenderStarSky() const
{
	for (int i = 0; i < 300; i++) 
	{
		DrawStar(Vec2(m_starsPositions[i].x, m_starsPositions[i].y), 1.f, 0.f);
	}

}

void Game::DrawStar(Vec2 const& position, float scale, float orientationDegree) const
{
	//int starAlpha = static_cast<int>(255 * SinDegrees(50.f * static_cast<float> (GetCurrentTimeSeconds())));
	int starAlpha = g_rng->RollRandomIntInRange(100, 255);
	//int starAlpha = 255;
	Vertex_PCU verts[3];
	verts[0].m_position = Vec3(0.2f, 0.f, 0.f);
	verts[1].m_position = Vec3(-0.2f, 0.2f, 0.f);
	verts[2].m_position = Vec3(-0.2f, -0.2f, 0.f);
	verts[0].m_color = Rgba8(255, 255, 255);
	verts[1].m_color = Rgba8(255, 255, 255);
	verts[2].m_color = Rgba8(255, 255, 255);
	verts[0].m_color.a = static_cast<unsigned char>(starAlpha);
	verts[1].m_color.a = static_cast<unsigned char>(starAlpha);
	verts[2].m_color.a = static_cast<unsigned char>(starAlpha);
	TransformVertexArrayXY3D( 3, verts, scale, orientationDegree, position);

	g_theRenderer->DrawVertexArray(3, verts);
}

void Game::CheckGameEndWithPlayerDie(float deltaSeconds)
{
	if (m_playerShip->GetNumExtraLives() < 0) 
	{
		if (!m_isVicOrLoseSoundPlayed)
		{
			g_theAudio->StopSound(m_backgroundmusic);
			SoundID playerLoseSound = g_theAudio->CreateOrGetSound("Data/Audio/gameover.wav");
			g_theAudio->StartSound(playerLoseSound, false, 2.f);
			m_isVicOrLoseSoundPlayed = true;
		}
		m_timerSwitch = true;
		if (PauseForThreeSeconds(deltaSeconds))
		{
			m_isAttractMode = true;
			m_timerSwitch = false;
		}
	}
}

void Game::DeleteGarbageEntities()
{
	for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; asteroidIndex++)
	{
		Asteroid*& asteroid = m_asteroids[asteroidIndex];
		if (asteroid && asteroid->m_isGarbage) 
		{
			delete asteroid;
			asteroid = nullptr;
		}
	}
	for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++) 
	{
		Bullet*& bullet = m_bullets[bulletIndex];
		if (bullet && bullet->m_isGarbage) 
		{
			delete bullet;
			bullet = nullptr;
		}
	}
	for (int beeltleIndex = 0; beeltleIndex < MAX_BEETLES; beeltleIndex++) 
	{
		Beetle*& beetle = m_beetles[beeltleIndex];
		if (beetle && beetle->m_isGarbage) 
		{
			delete beetle;
			beetle = nullptr;
		}
	}
	for (int waspIndex = 0; waspIndex < MAX_WASP; waspIndex++) 
	{
		Wasp*& wasp = m_wasps[waspIndex];
		if (wasp && wasp->m_isGarbage) 
		{
			delete wasp;
			wasp = nullptr;
		}
	}
	Prop*& prop = m_props[0];
	if (prop && prop->m_isGarbage)
	{
		delete prop;
		prop = nullptr;
	}

	for (int debrisIndex = 0; debrisIndex < MAX_DEBRIS; debrisIndex++)
	{
		Debris*& debris = m_debris[debrisIndex];
		if (debris && debris->m_isGarbage) 
		{
			delete debris;
			debris = nullptr;
		}
	}
}
