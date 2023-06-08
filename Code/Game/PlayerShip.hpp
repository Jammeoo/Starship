#pragma once
#include"Game/GameCommon.hpp"
#include"Game/Entity.hpp"
#include"Engine/Core/Vertex_PCU.hpp"
#include"Engine/Math/RandomNumberGenerator.hpp"


class Entity;

class PlayerShip :public Entity 
{
public:
	PlayerShip(Game* owner, Vec2 const& startPos);
	~PlayerShip();

	virtual void Update(float deltaSeconds) override;
	virtual void Render()const override;
	virtual void Die()override;
public:
	void InitializeLocalVerts();
	void InitializeLocalVerts(int numberOfVerts, Vertex_PCU* localVerts);
	void ChangeLocalTailAnimaVert();
	void ResetLocalTailAnimaVert();
	void BounceOffWalls();
	void Respawn();
	void UpdateFromKeyBoard(float deltaSeconds);
	void UpdateFromController(float deltaSeconds);
	int  GetNumExtraLives() const;
	bool IsDead();
private:
	Vertex_PCU m_localVerts[NUM_SHIP_VERTS];
	int		m_extraLives		= 3;
	float	m_thrustFraction	= 0.f;
	float	m_tailAnimaVertex	= 2.f;
	//bool	m_isTurningLeft		= false;
	//bool	m_isTurningRight	= false;
	bool	m_isThrusting		= false;
	bool	m_isDebrisSpawned	= false;
};