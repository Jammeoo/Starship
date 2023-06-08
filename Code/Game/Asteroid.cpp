#include "Asteroid.hpp"
#include"Engine/Math/MathUtils.hpp"
#include"Engine/Core/VertexUtils.hpp"
#include"Engine/Renderer/Renderer.hpp"

Asteroid::Asteroid(Game* owner, Vec2 const& startPos)
	:Entity(owner, startPos)
{
	m_health = 3;
	m_physicsRadius = ASTEROID_PHYSICS_RADIUS;
	m_cosmeticRadius = ASTEROID_COSMETIC_RADIUS;
	InitializeLocalVerts();
}

Asteroid::~Asteroid()
{

}

void Asteroid::Update(float deltaSeconds)
{
	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);
	if (IsOffscreen()) 
	{
		if (m_position.x > WORLD_SIZE_X + m_cosmeticRadius)
			m_position.x = 0 - m_cosmeticRadius;
		if (m_position.y > WORLD_SIZE_Y + m_cosmeticRadius)
			m_position.y = 0 - m_cosmeticRadius;
		if (m_position.x < 0 - m_cosmeticRadius)
			m_position.x = WORLD_SIZE_X + m_cosmeticRadius;
		if (m_position.y < 0 - m_cosmeticRadius)
			m_position.y = WORLD_SIZE_Y + m_cosmeticRadius;
	}

	
	if ( m_health <= 0)
	{
		Die();
	}
	
}


void Asteroid::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_ASTEROID_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_ASTEROID_VERTS; ++vertIndex) {
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}

	TransformVertexArrayXY3D(NUM_ASTEROID_VERTS, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_ASTEROID_VERTS, tempWorldVerts);
	RenderHealthBar();
}

void Asteroid::Die()
{
	SoundID enemyDieSound = g_theAudio->CreateOrGetSound("Data/Audio/Enemydie.wav");
	g_theAudio->StartSound(enemyDieSound, false, 1.f);
	m_isDead = true;
	m_isGarbage = true;
	m_game->m_numberOfDeadEnemy++;
	m_game->SpawnDebrisCluster(8, m_position, m_velocity, 1.5f, 0.5f, Rgba8(100, 100, 100));
}

void Asteroid::InitializeLocalVerts()
{
	float asteroidRadii[NUM_ASTEROID_SIDES] = {};
	for (int sideNum = 0; sideNum < NUM_ASTEROID_SIDES; ++sideNum) 
	{
		asteroidRadii[sideNum] = g_rng->RollRandomFloatInRange(m_physicsRadius, m_cosmeticRadius);
	}
	float degreePerAsteroidSide = 360.f / (float)NUM_ASTEROID_SIDES;
	//randomize degree and length per asteroid
	Vec2 asteroidLocalVertPositions[NUM_ASTEROID_SIDES] = {};
	for (int sideNum = 0; sideNum < NUM_ASTEROID_SIDES; ++sideNum) 
	{
		float degrees = degreePerAsteroidSide * static_cast<float>(sideNum);
		float radius = asteroidRadii[sideNum];
		asteroidLocalVertPositions[sideNum].x = radius * CosDegrees(degrees);
		asteroidLocalVertPositions[sideNum].y = radius * SinDegrees(degrees);
	}

	//Build trigonal
	for (int triNum = 0; triNum < NUM_ASTEROID_TRIS; ++triNum) 
	{
		int startRadiusIndex = triNum;
		int endRadiusIndex = (triNum + 1) % NUM_ASTEROID_SIDES;
		int firstVectIndex = (triNum * 3) + 0;
		int secondVectIndex= (triNum * 3) + 1;
		int thirdVectIndex = (triNum * 3) + 2;
		Vec2 secondVertOfs = asteroidLocalVertPositions[startRadiusIndex];
		Vec2 thirdVertOfs = asteroidLocalVertPositions[endRadiusIndex];
		m_localVerts[firstVectIndex].m_position = Vec3(0.f, 0.f, 0.f);
		m_localVerts[secondVectIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		m_localVerts[thirdVectIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);
	}
	//Set colors
	for (int vertIndex = 0; vertIndex < NUM_ASTEROID_VERTS; ++vertIndex) 
	{
		m_localVerts[vertIndex].m_color = Rgba8(100, 100, 100);
	}
}

void Asteroid::DrawHealthBar(Vec2 const& position, float scale, float orientationDegree)const
{
	Vertex_PCU healthBarVerts[6];
	healthBarVerts[0].m_position = Vec3(0.5f, -0.5f, 0.f);
	healthBarVerts[1].m_position = Vec3(0.5f, 0.5f, 0.f);
	healthBarVerts[2].m_position = Vec3(-0.5f, -0.5f, 0.f);

	healthBarVerts[3].m_position = Vec3(0.5f, 0.5f, 0.f);
	healthBarVerts[4].m_position = Vec3(-0.5f, -0.5f, 0.f);
	healthBarVerts[5].m_position = Vec3(-0.5f, 0.5f, 0.f);

	for (int BarVertsIndex = 0; BarVertsIndex < 6; BarVertsIndex++) 
	{
		healthBarVerts[BarVertsIndex].m_color = Rgba8(255, 0, 0);
	}
	TransformVertexArrayXY3D(6, healthBarVerts, scale, orientationDegree, position);
	g_theRenderer->DrawVertexArray( 6, healthBarVerts);
}

void Asteroid::DrawBarBackground(Vec2 const& position, float scale, float orientationDegree) const
{
	Vertex_PCU healthBarBackVerts[6];
	healthBarBackVerts[0].m_position = Vec3(-1.f, 0.75f, 0.f);
	healthBarBackVerts[1].m_position = Vec3(-1.f, -0.75f, 0.f);
	healthBarBackVerts[2].m_position = Vec3(1.f, -0.75f, 0.f);

	healthBarBackVerts[3].m_position = Vec3(-1.f, 0.75f, 0.f);
	healthBarBackVerts[4].m_position = Vec3(1.f, 0.75f, 0.f);
	healthBarBackVerts[5].m_position = Vec3(1.f, -0.75f, 0.f);

	for (int BarBackVertsIndex = 0; BarBackVertsIndex < 6; BarBackVertsIndex++) 
	{
		healthBarBackVerts[BarBackVertsIndex].m_color = Rgba8(210, 150, 150);
	}
	TransformVertexArrayXY3D(6, healthBarBackVerts, scale, orientationDegree, position);
	g_theRenderer->DrawVertexArray(6, healthBarBackVerts);
}

void Asteroid::RenderHealthBar() const
{
	float padding = 3.f;
	float stridex = 1.2f;
	float offset = 0.5f;
	int initialHealth = 3;
	for (int i = 0; i < initialHealth; ++i)
	{
		DrawBarBackground(Vec2(m_position.x - offset - stridex * static_cast<float>(i), m_position.y - padding), 1.f, 0.f);
	}
	for (int i = 0; i < m_health; ++i)
	{
		float x = m_position.x - padding + stridex * static_cast<float>(i);
		float y = m_position.y - padding;
		DrawHealthBar(Vec2(x, y), 1.f, 0.f);
	}
	
}

