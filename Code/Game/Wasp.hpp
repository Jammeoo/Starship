#pragma once
#include"Game/PlayerShip.hpp"
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Vec2.hpp"



class Wasp :public Entity 
{
public:
	Wasp(Game* owner, Vec2 const& startPos);
	~Wasp();
	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die()override;
private:
	void InitializeLocalVerts();
	void GetOrientationDegree();
private:
	Vertex_PCU m_localVerts[NUM_WASP_VERTS];
	Vec2 m_accel = Vec2(0.f, 0.f);
public:
	void DrawHealthBar(Vec2 const& position, float scale, float orientationDegree)const;
	void DrawBarBackground(Vec2 const& position, float scale, float orientationDegree)const;
	void RenderHealthBar()const;
};