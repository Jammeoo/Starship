#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "GameCommon.hpp"
#include "Game/Game.hpp"
class Game;

class Entity 
{
public:
	Entity(Game* owner, Vec2 const& startPos);
	virtual ~Entity();
	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const= 0;
	virtual void DebugRender()const;
	virtual void Die() = 0;
	bool IsOffscreen() const;
	Vec2 GetForwardNormal() const;
	
public:
	
	Vec2 m_position;
	Vec2 m_velocity;
	float m_orientationDegrees		= 0.f;
	float m_angularVelocity			= 0.f;
	float m_physicsRadius			= 2.f;
	float m_cosmeticRadius			= 5.f;
	int m_health					= 1;
	bool m_isDead					= false;
	bool m_isGarbage				= false;
	Game* m_game = nullptr;
};