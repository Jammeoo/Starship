#pragma once
#include"Engine/Math/RandomNumberGenerator.hpp"
#include"Game/Entity.hpp"
#include "GameCommon.hpp"
#include"Engine/Core/Vertex_PCU.hpp"
#include"Engine/Math/Vec2.hpp"
//---------------------------------

class Asteroid :public Entity 
{
public:
	Asteroid(Game* owner, Vec2 const& startPos);	
	~Asteroid();
	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die()override;
private:
	void InitializeLocalVerts();
private:
	Vertex_PCU m_localVerts[NUM_ASTEROID_VERTS];
public:
	void DrawHealthBar(Vec2 const& position, float scale, float orientationDegree)const;
	void DrawBarBackground(Vec2 const& position, float scale, float orientationDegree)const;
	void RenderHealthBar()const;
	
};