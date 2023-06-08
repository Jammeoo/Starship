#include "Game/Wasp.hpp"
class Game;
Wasp::Wasp(Game* owner, Vec2 const& startPos)
	:Entity(owner, startPos)
{
	m_health = 2;
	m_physicsRadius = WASP_PHYSICS_RADIUS;
	m_cosmeticRadius = WASP_COSMETIC_RADIUS;
	InitializeLocalVerts();
}

Wasp::~Wasp()
{

}

void Wasp::Update(float deltaSeconds)
{
	if (!m_game->m_playerShip->IsDead())
	{
		m_position += (m_velocity * deltaSeconds);
		GetOrientationDegree();
		m_accel = Vec2::MakeFromPolarDegrees(m_orientationDegrees, WASP_SPEED);
		m_velocity += m_accel * deltaSeconds;
	}
	if (m_health <= 0)
	{
		Die();
	}
}

void Wasp::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_WASP_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_WASP_VERTS; vertIndex++) 
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_WASP_VERTS, tempWorldVerts, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_WASP_VERTS, tempWorldVerts);
	RenderHealthBar();
}

void Wasp::Die()
{
	SoundID enemyDieSound = g_theAudio->CreateOrGetSound("Data/Audio/Enemydie.wav");
	g_theAudio->StartSound(enemyDieSound, false, 1.f);
	m_isDead = true;
	m_isGarbage = true;
	m_game->m_numberOfDeadEnemy++;
	m_game->SpawnDebrisCluster(8, m_position, m_velocity, 1.4f, 0.3f, Rgba8(255, 204, 0));
}

void Wasp::InitializeLocalVerts()
{
	//Up half
	m_localVerts[0].m_position = Vec3(2.f, 0.f, 0.f);
	m_localVerts[1].m_position = Vec3(0.f, 2.f, 0.f);
	m_localVerts[2].m_position = Vec3(0.f, -2.f, 0.f);
	//Down half 
	m_localVerts[3].m_position = Vec3(0.f, 1.f, 0.f);
	m_localVerts[4].m_position = Vec3(0.f, -1.f, 0.f);
	m_localVerts[5].m_position = Vec3(-2.f, 0.f, 0.f);


	for (int vertIndex = 0; vertIndex < NUM_WASP_VERTS; ++vertIndex) 
	{
		m_localVerts[vertIndex].m_color = Rgba8(255, 204, 0);
	}
}

void Wasp::GetOrientationDegree()
{
	Vec2 facingdir = (m_game->m_playerShip->m_position - m_position).GetNormalized();
	m_orientationDegrees = Atan2Degrees(facingdir.y, facingdir.x);
}

void Wasp::DrawHealthBar(Vec2 const& position, float scale, float orientationDegree) const
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
	g_theRenderer->DrawVertexArray(6, healthBarVerts);
}

void Wasp::DrawBarBackground(Vec2 const& position, float scale, float orientationDegree) const
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

void Wasp::RenderHealthBar() const
{
	float padding = 3.f;
	float stridex = 1.2f;
	float offset = 3.0f;
	int initialHealth = 2;
	for (int i = 0; i < initialHealth; ++i)
	{
		DrawBarBackground(Vec2(m_position.x - offset + stridex * static_cast<float>(i), m_position.y - padding), 1.f, 0.f);
	}
	for (int i = 0; i < m_health; ++i)
	{
		float x = m_position.x - padding + stridex * static_cast<float>(i);
		float y = m_position.y - padding;
		DrawHealthBar(Vec2(x, y), 1.f, 0.f);
	}
}

