#pragma once
#include"Game/GameCommon.hpp"
#include"Game/Entity.hpp"
#include"Engine/Core/Vertex_PCU.hpp"
class Game;

//-----------------------------------------------------------


//-----------------------------------------------------------

class Bullet :public Entity 
{

public:
	Bullet(Game* owner, Vec2 const& startPos);
	~Bullet();
	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die() override;

private:
	void InitializeLocalVerts();
private:
	Vertex_PCU m_localVerts[NUM_BULLET_VERTS];

public:
	float m_naviRadius = 0;
};