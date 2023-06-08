#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/Vec2.hpp"

class Prop :public Entity 
{
public:
	Prop(Game* owner, Vec2 const& startPos);
	~Prop();
	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die()override;
private:
	void InitializeLocalVerts();
private:
	Vertex_PCU m_localVerts[NUM_PROP_VERTS];
};