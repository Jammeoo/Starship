#include<math.h>
#include"Engine/Core/Rgba8.hpp"
#include"Engine/Math/MathUtils.hpp"
#include "Game/Entity.hpp"

Entity::Entity(Game* owner, Vec2 const& startPos)
	:m_game(owner)
	,m_position(startPos)
{
}

Entity::~Entity()
{
}

void Entity::DebugRender()const 
{
	float lineThickness = 0.1f;
	float ringThickness = 0.2f;
	float redLineX = CosDegrees(m_orientationDegrees) * m_cosmeticRadius;
	float redLineY = SinDegrees(m_orientationDegrees) * m_cosmeticRadius;
	float greenLineX = CosDegrees(m_orientationDegrees + 90.f) * m_cosmeticRadius;
	float greenLineY = SinDegrees(m_orientationDegrees + 90.f) * m_cosmeticRadius;
	DebugDrawRing(m_position, m_physicsRadius, ringThickness, Rgba8(0, 255, 255));
	DebugDrawRing(m_position, m_cosmeticRadius, ringThickness, Rgba8(255, 0, 255));
	DebugDrawLine(m_position, m_position + m_velocity, lineThickness, Rgba8(255, 255, 0));
	DebugDrawLine(m_position, m_position + Vec2(redLineX, redLineY), lineThickness, Rgba8(255, 0, 0));
	DebugDrawLine(m_position, m_position + Vec2(greenLineX, greenLineY), lineThickness, Rgba8(0, 255, 0));
	
}

bool Entity::IsOffscreen()const 
{
	if (m_position.x > WORLD_SIZE_X + m_cosmeticRadius)
		return true;
	if (m_position.y > WORLD_SIZE_Y + m_cosmeticRadius)
		return true;
	if (m_position.x < -m_cosmeticRadius)
		return true;
	if (m_position.y < -m_cosmeticRadius)
		return true;
	return false;
}

Vec2 Entity::GetForwardNormal()const 
{
	return Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));
}
