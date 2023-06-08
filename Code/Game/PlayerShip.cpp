#include "Game/PlayerShip.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"

#define UNUSED(x) (void)(x);
extern App* g_theApp;
extern Renderer* g_theRenderer;
extern InputSystem* g_theInput;
extern AudioSystem* g_theAudio;

PlayerShip::PlayerShip(Game* owner, Vec2 const& startPos)
	:Entity(owner, startPos)
{
	m_extraLives		= 3;
	m_physicsRadius		= PLAYER_SHIP_PHYSICS_RADIUS;
	m_cosmeticRadius	= PLAYER_SHIP_COSMETIC_RADIUS;
	m_isDebrisSpawned	= false;
	InitializeLocalVerts();
}

PlayerShip::~PlayerShip()
{
}

void PlayerShip::Update(float deltaSeconds)
{
	if (m_isThrusting) 
	{
		ChangeLocalTailAnimaVert();
	}
	else 
	{
		ResetLocalTailAnimaVert();
	}
	UpdateFromKeyBoard(deltaSeconds);
	UpdateFromController(deltaSeconds);
	BounceOffWalls();
	//m_velocity += Vec2(getdirection.normalize()*m_thrustFraction*deltaSeconds,
	m_position += (m_velocity * deltaSeconds);
	if (m_health <= 0) 
	{
		if (m_isDead == false)
		{
			SoundID deathSound = g_theAudio->CreateOrGetSound("Data/Audio/PlayerDead.wav");
			g_theAudio->StartSound(deathSound, false, 1.f);
		}
		Die();
	}
}

void PlayerShip::Render() const
{
	if (m_isDead) 
		return;
	Vertex_PCU tempShipWorldVerts[NUM_SHIP_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_SHIP_VERTS; ++vertIndex) 
	{
		tempShipWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
		TransformVertexArrayXY3D(NUM_SHIP_VERTS, tempShipWorldVerts, 1.f, m_orientationDegrees, m_position);
		g_theRenderer->DrawVertexArray(NUM_SHIP_VERTS, tempShipWorldVerts);
// 	if (m_isThrusting) 
// 	{
// 		TransformVertexArrayXY3D(NUM_SHIP_VERTS, tempShipWorldVerts, 1.f, m_orientationDegrees, m_position);
// 		g_theRenderer->DrawVertexArray(NUM_SHIP_VERTS, tempShipWorldVerts);
// 	}
// 	else
// 	{
// 		TransformVertexArrayXY3D(NUM_SHIP_VERTS - 3, tempShipWorldVerts, 1.f, m_orientationDegrees, m_position);
// 		g_theRenderer->DrawVertexArray(NUM_SHIP_VERTS - 3, tempShipWorldVerts);
// 	}

}

void PlayerShip::Die()
{
	m_isDead = true;
	m_game->m_isShakingScreen = true;
	if (m_isDebrisSpawned == true) 
		return;
	m_extraLives--;
	m_game->SpawnDebrisCluster(30, m_position, m_velocity, 2.0f, 0.5f, Rgba8(102, 153, 204));
	m_isDebrisSpawned = true;
}

void PlayerShip::InitializeLocalVerts()
{
	//Nose cone
	m_localVerts[0].m_position = Vec3(1.f, 0.f, 0.f);
	m_localVerts[1].m_position = Vec3(0.f, 1.f, 0.f);
	m_localVerts[2].m_position = Vec3(0.f, -1.f, 0.f);
	//Left wing 
	m_localVerts[3].m_position = Vec3(2.f, 1.f, 0.f);
	m_localVerts[4].m_position = Vec3(0.f, 2.f, 0.f);
	m_localVerts[5].m_position = Vec3(-2.f, 1.f, 0.f);
	//Right wing
	m_localVerts[6].m_position = Vec3(2.f, -1.f, 0.f);
	m_localVerts[7].m_position = Vec3(-2.f, -1.f, 0.f);
	m_localVerts[8].m_position = Vec3(0.f, -2.f, 0.f);
	//Body(quad triangle 1 of 2)
	m_localVerts[9].m_position = Vec3(0.f, 1.f, 0.f);
	m_localVerts[10].m_position = Vec3(-2.f, -1.f, 0.f);
	m_localVerts[11].m_position = Vec3(0.f, -1.f, 0.f);
	//Body(quad triangle 2 of 2)
	m_localVerts[12].m_position = Vec3(0.f, 1.f, 0.f);
	m_localVerts[13].m_position = Vec3(-2.f, 1.f, 0.f);
	m_localVerts[14].m_position = Vec3(-2.f, -1.f, 0.f);

	m_localVerts[15].m_position = Vec3(-2.f, 1.f, 0.f);
	m_localVerts[16].m_position = Vec3(-2.f, -1.f, 0.f);
	m_localVerts[17].m_position = Vec3(-8.f, 0.f, 0.f);//Will be changed in the vertex animation
	//except last 3 vertices
	for (int vertIndex = 0; vertIndex < NUM_SHIP_VERTS; ++vertIndex) 
	{
		m_localVerts[vertIndex].m_color = Rgba8(102, 153, 204);
	}
	m_localVerts[15].m_color = Rgba8(255, 0, 0);
	m_localVerts[16].m_color = Rgba8(255, 0, 0);
	m_localVerts[17].m_color = Rgba8(0, 0, 0, 0);
}


void PlayerShip::InitializeLocalVerts(int numberOfVerts, Vertex_PCU* localVerts)
{
	//Nose cone
	localVerts[numberOfVerts - 15].m_position = Vec3(1.f, 0.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 14].m_position = Vec3(0.f, 1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 13].m_position = Vec3(0.f, -1.f, 0.f) * 8.f;
	//Left wing 
	localVerts[numberOfVerts - 12].m_position = Vec3(2.f, 1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 11].m_position = Vec3(0.f, 2.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 10].m_position = Vec3(-2.f, 1.f, 0.f) * 8.f;
	//Right wing
	localVerts[numberOfVerts - 9].m_position = Vec3(2.f, -1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 8].m_position = Vec3(-2.f, -1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 7].m_position = Vec3(0.f, -2.f, 0.f) * 8.f;
	//Body(quad triangle 1 of 2)
	localVerts[numberOfVerts - 6].m_position = Vec3(0.f, 1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 5].m_position = Vec3(-2.f, -1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 4].m_position = Vec3(0.f, -1.f, 0.f) * 8.f;
	//Body(quad triangle 2 of 2)
	localVerts[numberOfVerts - 3].m_position = Vec3(0.f, 1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 2].m_position = Vec3(-2.f, 1.f, 0.f) * 8.f;
	localVerts[numberOfVerts - 1].m_position = Vec3(-2.f, -1.f, 0.f) * 8.f;

	for (int vertIndex = 0; vertIndex < numberOfVerts; ++vertIndex) 
	{
		localVerts[vertIndex].m_color = Rgba8(102, 153, 204);
	}
}

void PlayerShip::ChangeLocalTailAnimaVert()
{
	float randomTail = g_rng->RollRandomFloatInRange(-3.f, -8.f);
	m_localVerts[17].m_position = Vec3(randomTail, 0.f, 0.f);
}

void PlayerShip::ResetLocalTailAnimaVert()
{
	m_localVerts[17].m_position = Vec3(-2.f, 0.f, 0.f);
}

void PlayerShip::BounceOffWalls()
{
	//Normal boundary
	if (m_position.x + m_cosmeticRadius >= WORLD_SIZE_X || m_position.x <= m_cosmeticRadius + 0) 
	{
		m_velocity.x = -m_velocity.x;
	}
	if (m_position.y + m_cosmeticRadius >= WORLD_SIZE_Y || m_position.y <= m_cosmeticRadius + 0) 
	{
		m_velocity.y = -m_velocity.y;
	}
	//Four points
	if ((m_position.x + m_cosmeticRadius >= WORLD_SIZE_X && m_position.y + m_cosmeticRadius >= WORLD_SIZE_Y) ||
		(m_position.x <= m_cosmeticRadius && m_position.y <= m_cosmeticRadius) ||
		(m_position.x <= m_cosmeticRadius && m_position.y + m_cosmeticRadius >= WORLD_SIZE_Y) ||
		(m_position.x + m_cosmeticRadius >= WORLD_SIZE_X && m_position.y <= m_cosmeticRadius)) 
	{
		m_velocity.y = -m_velocity.y;
		m_velocity.x = -m_velocity.x;
	}
}

void PlayerShip::Respawn()
{
	SoundID rebirthSound = g_theAudio->CreateOrGetSound("Data/Audio/ReBirth.mp3");
	g_theAudio->StartSound(rebirthSound, false, 1.f);
	m_position = Vec2(WORLD_CENTER_X, WORLD_CENTER_Y);
	m_velocity = Vec2(0.f, 0.f);
	m_thrustFraction = 0.f;
	m_orientationDegrees = 0.f;
	m_health = 1;
	m_isDead = false;
	m_isDebrisSpawned = false;
}

void PlayerShip::UpdateFromKeyBoard(float deltaSeconds)
{
	if (g_theInput->WasKeyJustPressed(112))
	{
		m_game->m_isDebugDraw = !m_game->m_isDebugDraw;
	}
	if (g_theInput->IsKeyDown('S')) 
	{
		m_orientationDegrees += PLAYER_SHIP_TURN_SPEED * deltaSeconds;
	}
	if (g_theInput->IsKeyDown('F'))
	{
		m_orientationDegrees -= PLAYER_SHIP_TURN_SPEED * deltaSeconds;
	}
	if (g_theInput->IsKeyDown('E')) 
	{
		m_velocity += GetForwardNormal() * PLAYER_SHIP_ACCELERATION * deltaSeconds;
		m_isThrusting = true;
	}
	if (g_theInput->WasKeyJustReleased('E'))
	{
		m_isThrusting = false;
	}
	if (g_theInput->WasKeyJustPressed(' ')) 
	{
			Vec2 spawnBulletPosition = m_position + GetForwardNormal();
			m_game->SpawnBullet(spawnBulletPosition, m_orientationDegrees);
			SoundID shootSound = g_theAudio->CreateOrGetSound("Data/Audio/Laser_Shoot4.wav");
			g_theAudio->StartSound(shootSound, false, 0.5f);
			
	}
	if (g_theInput->WasKeyJustPressed('N')) 
	{
		if (m_isDead && m_extraLives >= 0) 
		{
			Respawn();
			
		}
	}
	if (g_theInput->WasKeyJustPressed('I'))
	{
		m_game->SpawmRandomAsteroid();
	}
	if (g_theInput->WasKeyJustPressed('V'))
	{
		m_game->m_isNaviMode = true;
	}
}

void PlayerShip::UpdateFromController(float deltaSeconds)
{
	UNUSED(deltaSeconds);
	XboxController const& controller = g_theInput->GetController(0);

	//Respawn
	if (m_isDead)
	{
		if (controller.WasButtonJustPressed(XboxButtonID::XBOX_BUTTON_START) && m_extraLives > 0) 
		{
			Respawn();
		}
		return;
	}
	//Drive
	float leftStickMagnitude = controller.GetLeftStick().GetMagnitude();
	
	if (leftStickMagnitude > 0.f) 
	{
		m_thrustFraction = leftStickMagnitude * PLAYER_SHIP_ACCELERATION;
		m_velocity += GetForwardNormal() * m_thrustFraction * deltaSeconds;
		m_isThrusting = true;
		m_orientationDegrees = controller.GetLeftStick().GetOrientationDegrees();
	}
	if (!(leftStickMagnitude > 0.f) && (controller.IsConnected() == true)) //
	{
		m_thrustFraction -= 1.f;
		m_isThrusting = false;
	}
	//Shoot
	if (controller.WasButtonJustPressed(XboxButtonID::XBOX_BUTTON_A)) 
	{
		Vec2 forwardNormal = GetForwardNormal();
		Vec2 nosePosition = m_position + (forwardNormal * 1.f);
		m_game->SpawnBullet(nosePosition, m_orientationDegrees);
		SoundID shootSound = g_theAudio->CreateOrGetSound("Data/Audio/Laser_Shoot4.wav");
		g_theAudio->StartSound(shootSound,false,0.5f);
	}
}

int PlayerShip::GetNumExtraLives() const
{
	return m_extraLives;
}

bool PlayerShip::IsDead()
{
	return m_isDead;
}
