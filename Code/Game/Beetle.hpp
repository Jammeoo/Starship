#pragma once
#include"Game/PlayerShip.hpp"
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Vec2.hpp"



class Beetle :public Entity 
{
public:
	Beetle(Game* owner, Vec2 const& startPos);
	~Beetle();
	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die()override;
private:
	void InitializeLocalVerts();
	void GetOrientationDegree();
private:
	Vertex_PCU m_localVerts[NUM_BEETLE_VERTS];
public:
	void DrawHealthBar(Vec2 const& position, float scale, float orientationDegree)const;
	void DrawBarBackground(Vec2 const& position, float scale, float orientationDegree)const;
	void RenderHealthBar()const;
};