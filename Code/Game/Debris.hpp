#pragma once
#include "Game/PlayerShip.hpp"
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Vec2.hpp"



class Debris :public Entity 
{
public:
	Debris(Game* owner, Vec2 const& startPos, Vec2 velocity, float maxRadius, float minRadius, Rgba8 color);
	~Debris();
	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die()override;
private:
	void InitializeLocalVerts(float maxRadius, float minRadius, Rgba8 color);

private:
	int m_sideNum = 0;
	int m_vertsNum = 0;
	float m_lifeTime = 0.f;
	
	Vertex_PCU m_localVerts[NUM_DEBRIS_MAX_VERTS];
};