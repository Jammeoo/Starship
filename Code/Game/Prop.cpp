#include"Game/Prop.hpp"
Prop::Prop(Game* owner, Vec2 const& startPos)
	:Entity(owner, startPos)
{
	m_physicsRadius = PROP_PHYSICS_RADIUS;
	m_cosmeticRadius = PROP_COSMETIC_RADIUS;
	float x = g_rng->RollRandomFloatInRange(5, 10);
	float y = g_rng->RollRandomFloatInRange(5, 10);
	m_velocity = Vec2(x, y);
	m_angularVelocity = 5.0f;
	InitializeLocalVerts();
	m_health = 1;
}

Prop::~Prop()
{

}

void Prop::Update(float deltaSeconds)
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
	if (m_health <= 0)
	{
		Die();
	}
}

void Prop::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_PROP_VERTS];
	for (int vertIndex = 0; vertIndex < NUM_PROP_VERTS; ++vertIndex)
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}
	TransformVertexArrayXY3D(NUM_PROP_VERTS, tempWorldVerts, 2.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_PROP_VERTS, tempWorldVerts);
}

void Prop::Die()
{
	m_isDead = true;
	m_isGarbage = true;
	SoundID WeaponLevelUp = g_theAudio->CreateOrGetSound("Data/Audio/levelup.wav");
	g_theAudio->StartSound(WeaponLevelUp, false, 1.f);
}

void Prop::InitializeLocalVerts()
{
	//top hat
	m_localVerts[0].m_position = Vec3(1.f, 1.f, 0.f);
	m_localVerts[1].m_position = Vec3(0.f, 2.f, 0.f);
	m_localVerts[2].m_position = Vec3(-1.f, 1.f, 0.f);
	//top cover 1 
	m_localVerts[3].m_position = Vec3(2.f, 1.f, 0.f);
	m_localVerts[4].m_position = Vec3(-2.f, 1.f, 0.f);
	m_localVerts[5].m_position = Vec3(2.f, 0.5f, 0.f);
	//top cover 2
	m_localVerts[6].m_position = Vec3(2.f, 0.5f, 0.f);
	m_localVerts[7].m_position = Vec3(-2.f, 1.f, 0.f);
	m_localVerts[8].m_position = Vec3(-2.f, 0.5f, 0.f);
	//top half body
	m_localVerts[9].m_position = Vec3(1.f, 0.5f, 0.f);
	m_localVerts[10].m_position = Vec3(1.f, -1.f, 0.f);
	m_localVerts[11].m_position = Vec3(1.f, 0.5f, 0.f);
	//bottom half body 
	m_localVerts[12].m_position = Vec3(1.f, -1.f, 0.f);
	m_localVerts[13].m_position = Vec3(1.f, 0.5f, 0.f);
	m_localVerts[14].m_position = Vec3(-1.f, -1.f, 0.f);

	for (int vertIndex = 0; vertIndex < NUM_PROP_VERTS; ++vertIndex)
	{
		int r = g_rng->RollRandomIntInRange(120, 255);
		int g = g_rng->RollRandomIntInRange(120, 255);
		int b = g_rng->RollRandomIntInRange(120, 255);
		m_localVerts[vertIndex].m_color = Rgba8(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b));
	}
}

