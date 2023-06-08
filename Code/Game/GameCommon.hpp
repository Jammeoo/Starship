#pragma once
#include"GameCommon.hpp"
#include"Engine/Math/Vec2.hpp"
#include"Engine/Core/Rgba8.hpp"
#include"Engine/Core/Vertex_PCU.hpp"
#include"Engine/Input/InputSystem.hpp"
#include"Engine/Audio/AudioSystem.hpp"
#include"Engine/Renderer/Renderer.hpp"

class	App;
struct	Vec2;
struct	Rgba8;
class	Renderer;
class	RandomNumberGenerator;

extern App* g_theApp;
extern Renderer* g_theRenderer;
extern InputSystem* g_theInput;
extern AudioSystem* g_theAudio;
extern RandomNumberGenerator* g_rng;


constexpr float WORLD_SIZE_X = 200.f;
constexpr float WORLD_SIZE_Y = 100.f;
constexpr float WORLD_CENTER_X = WORLD_SIZE_X / 2.f;
constexpr float WORLD_CENTER_Y = WORLD_SIZE_Y / 2.f;


constexpr float BEETLE_SPEED = 8.f;
constexpr int NUM_STARTING_BEETLE = 2;
constexpr int MAX_BEETLES = 30;
constexpr float BEETLE_PHYSICS_RADIUS = 1.5f;
constexpr float BEETLE_COSMETIC_RADIUS = 2.0f;
constexpr int NUM_BEETLE_MAX_TRIS = 2;
constexpr int NUM_BEETLE_VERTS = 3 * NUM_BEETLE_MAX_TRIS;

constexpr float WASP_SPEED = 2.f;
constexpr int NUM_STARTING_WASP = 1;
constexpr int MAX_WASP = 30;
constexpr float WASP_PHYSICS_RADIUS = 1.5f;
constexpr float WASP_COSMETIC_RADIUS = 2.0f;
constexpr int NUM_WASP_TRIS = 2;
constexpr int NUM_WASP_VERTS = 3 * NUM_WASP_TRIS;

constexpr int MAX_DEBRIS = 400;
constexpr float DEBRIS_PHYSICS_RADIUS = 0.2f;
constexpr float DEBRIS_COSMETIC_RADIUS = 0.6f;
constexpr int NUM_DEBRIS_MAX_TRIS = 8;
constexpr int NUM_DEBRIS_MAX_SIDES = NUM_DEBRIS_MAX_TRIS;
constexpr int NUM_DEBRIS_MAX_VERTS = 3 * NUM_DEBRIS_MAX_TRIS;
constexpr float DEBRIS_MAX_LIFETIME = 2.0f;

constexpr float ASTEROID_SPEED = 10.f;
constexpr float ASTEROID_PHYSICS_RADIUS = 1.6f;
constexpr float ASTEROID_COSMETIC_RADIUS = 2.0f;
constexpr int NUM_STARTING_ASTEROIDS = 6;
constexpr int MAX_ASTEROIDS = 200;
constexpr int NUM_ASTEROID_SIDES = 16;
constexpr int NUM_ASTEROID_TRIS = NUM_ASTEROID_SIDES;
constexpr int NUM_ASTEROID_VERTS = 3 * NUM_ASTEROID_TRIS;

constexpr int MAX_BULLETS = 100;
constexpr float BULLET_LIFETIME_SECONDS = 2.0f;
constexpr float BULLET_SPEED = 80.f;
constexpr float BULLET_PHYSICS_RADIUS = 0.5f;
constexpr float BULLET_COSMETIC_RADIUS = 2.0f;
constexpr float BULLET_NAVI_RADIUS = 5.0f;
constexpr int NUM_BULLET_TRIS = 2;
constexpr int NUM_BULLET_VERTS = 3 * NUM_BULLET_TRIS;

constexpr float PLAYER_SHIP_ACCELERATION = 30.f;
constexpr float PLAYER_SHIP_TURN_SPEED = 300.f;
constexpr float PLAYER_SHIP_PHYSICS_RADIUS = 1.75f;
constexpr float PLAYER_SHIP_COSMETIC_RADIUS = 2.25f;
constexpr int NUM_SHIP_TRIS = 6;
constexpr int NUM_SHIP_VERTS = 3 * NUM_SHIP_TRIS;

constexpr float PROP_SPEED = 10.f;
constexpr float PROP_PHYSICS_RADIUS = 2.0f;
constexpr float PROP_COSMETIC_RADIUS = 3.f;
constexpr int MAX_PROP = 1;
constexpr int NUM_PROP_SIDES = 5;
constexpr int NUM_PROP_TRIS = NUM_ASTEROID_SIDES;
constexpr int NUM_PROP_VERTS = 3 * NUM_PROP_TRIS;

void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DebugDrawLine(Vec2 const& startPoint, Vec2 const& endPoint, float radius, Rgba8 const& color);
void DebugDrawLine(Vec2 const& startPoint, float DegreeTowardEndDirection, float length, float radius, Rgba8 const& color);