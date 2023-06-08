#include"Game/Bullet.hpp"

Bullet::Bullet(Game* owner, Vec2 const& startPos) 
	:Entity (owner, startPos)
{
	m_physicsRadius = BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = BULLET_COSMETIC_RADIUS;
	m_naviRadius = BULLET_NAVI_RADIUS;
	InitializeLocalVerts();
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaSeconds)
{
	m_position += (m_velocity * deltaSeconds);
	if (IsOffscreen() || m_health <= 0)
	{
		Die();
	}
}

void Bullet::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_BULLET_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_BULLET_VERTS; vertIndex++) 
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_BULLET_VERTS, tempWorldVerts, 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_BULLET_VERTS, tempWorldVerts);
}

void Bullet::Die()
{
	m_isDead = true;
	m_isGarbage = true;
	if (IsOffscreen()) 
		return;
	m_game->SpawnDebrisCluster(2, m_position, -0.4f * m_velocity, 0.8f, 0.4f, Rgba8(255, 255, 0));
}

void Bullet::InitializeLocalVerts()
{
	//Nose cone
	m_localVerts[0].m_position = Vec3(0.5f, 0.0f, 0.0f);
	m_localVerts[1].m_position = Vec3(0.0f, 0.5f, 0.0f);
	m_localVerts[2].m_position = Vec3(0.0f, -0.5f, 0.0f);
	m_localVerts[0].m_color = Rgba8(255,255,255,255);
	m_localVerts[1].m_color = Rgba8(255,255,0,255);
	m_localVerts[2].m_color = Rgba8(255,255,0,255);

	//Tail
	m_localVerts[3].m_position = Vec3(0.0f, -0.5f, 0.0f);
	m_localVerts[4].m_position = Vec3(0.0f, 0.5f, 0.0f);
	m_localVerts[5].m_position = Vec3(-2.0f, 0.0f, 0.0f);
	m_localVerts[3].m_color = Rgba8(255, 0, 0, 255);
	m_localVerts[4].m_color = Rgba8(255, 0, 0, 255);
	m_localVerts[5].m_color = Rgba8(255, 0, 0, 0);
}
