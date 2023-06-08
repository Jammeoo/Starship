#include"Game/Debris.hpp"
#include"Engine/Math/MathUtils.hpp"
#include"Engine/Core/VertexUtils.hpp"
#include"Engine/Renderer/Renderer.hpp"

Debris::Debris(Game* owner, Vec2 const& startPos, Vec2 velocity,float maxRadius, float minRadius, Rgba8 color)
	:Entity(owner,startPos)
{
	m_sideNum = g_rng->RollRandomIntInRange(3, NUM_DEBRIS_MAX_TRIS);
	m_lifeTime = 0.f;
	m_velocity = velocity;
	m_angularVelocity = 1.0f;
	m_vertsNum = 3 * m_sideNum;
	InitializeLocalVerts(maxRadius,minRadius,color);
}



Debris::~Debris()
{

}

void Debris::Update(float deltaSeconds)
{
	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);
	m_lifeTime += deltaSeconds;
	if (IsOffscreen() || m_lifeTime >= DEBRIS_MAX_LIFETIME)
	{
		Die();
	}
	for (int debrisIndex=0; debrisIndex < m_vertsNum; debrisIndex++)
	{
		m_localVerts[debrisIndex].m_color.a = static_cast<unsigned char>((1 - (m_lifeTime / DEBRIS_MAX_LIFETIME)) * 127.f);
	}
	
}

void Debris::Render() const
{
	Vertex_PCU tempWorldVerts[NUM_DEBRIS_MAX_VERTS];
	for (int vertIndex = 0; vertIndex < m_sideNum * 3; ++vertIndex) 
	{
		tempWorldVerts[vertIndex] = m_localVerts[vertIndex];
	}

	TransformVertexArrayXY3D(m_sideNum * 3, &tempWorldVerts[0], 1.f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(m_sideNum * 3, tempWorldVerts);
}

void Debris::Die()
{
	m_isDead = true;
	m_isGarbage = true;
}

void Debris::InitializeLocalVerts(float maxRadius, float minRadius, Rgba8 color)
{
	float debrisRadii[NUM_DEBRIS_MAX_SIDES] = {};
	for (int sideNum = 0; sideNum < NUM_DEBRIS_MAX_SIDES; ++sideNum) 
	{
		debrisRadii[sideNum] = g_rng->RollRandomFloatInRange(minRadius, maxRadius);
	}
	float degreePerDebrisSide = 360.f / static_cast<float>(m_sideNum);
	//randomize degree and length per Debris
	Vec2 debrisLocalVertPositions[NUM_DEBRIS_MAX_SIDES] = {};
	for (int sideNum = 0; sideNum < NUM_DEBRIS_MAX_SIDES; ++sideNum) 
	{
		float degrees = degreePerDebrisSide * static_cast<float>(sideNum);
		float radius = debrisRadii[sideNum];
		debrisLocalVertPositions[sideNum].x = radius * CosDegrees(degrees);
		debrisLocalVertPositions[sideNum].y = radius * SinDegrees(degrees);
	}
	//determine the outside points
	for (int triNum = 0; triNum < NUM_DEBRIS_MAX_TRIS; ++triNum) 
	{
		int startRadiusIndex = triNum;
		int endRadiusIndex = (triNum + 1) % NUM_DEBRIS_MAX_SIDES;
		int firstVectIndex = (triNum * 3) + 0;
		int secondVectIndex = (triNum * 3) + 1;
		int thirdVectIndex = (triNum * 3) + 2;
		Vec2 secondVertOfs = debrisLocalVertPositions[startRadiusIndex];
		Vec2 thirdVertOfs = debrisLocalVertPositions[endRadiusIndex];
		m_localVerts[firstVectIndex].m_position = Vec3(0.f, 0.f, 0.f);
		m_localVerts[secondVectIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		m_localVerts[thirdVectIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);
	}
	//Set colors
	for (int vertIndex = 0; vertIndex < NUM_DEBRIS_MAX_VERTS; ++vertIndex) 
	{
		m_localVerts[vertIndex].m_color = color;
		m_localVerts[vertIndex].m_color.a = 127;
	}
}
