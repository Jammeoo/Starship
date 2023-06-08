#include "Game/GameCommon.hpp"

void DebugDrawRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;
	float innerRadius = radius - halfThickness;
	float outerRadius = radius + halfThickness;
	constexpr int NUM_SIDES = 16;                    
	constexpr int NUM_TRIS = 2 * NUM_SIDES;          //each side is a trapezoid
 	constexpr int NUM_VERTS = 3 * NUM_TRIS;          //3 vertexes per triangle

	Vertex_PCU verts[NUM_VERTS];
	constexpr float DEFREE_PER_SIDE = 360.f / static_cast<float>(NUM_SIDES);
	for (int sideNum = 0; sideNum < NUM_SIDES;sideNum++)
	{

		//Compute angle-related terms
		float startDegrees = DEFREE_PER_SIDE * static_cast<float>(sideNum);
		float endDegrees = DEFREE_PER_SIDE * static_cast<float>(sideNum + 1);
		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		//Compute inner & outer positions
		Vec3 innerStartPos(center.x + innerRadius * cosStart, center.y + innerRadius * sinStart, 0.f);
		Vec3 outerStartPos(center.x + outerRadius * cosStart, center.y + outerRadius * sinStart, 0.f);
		Vec3 outerEndPos(center.x + outerRadius * cosEnd, center.y + outerRadius * sinEnd, 0.f);
		Vec3 innerEndPos(center.x + innerRadius * cosEnd, center.y + innerRadius * sinEnd, 0.f);

		//Trapezoid is made of two trangles; ABC+DEF
		//A is inner end; B is inner start; C is outer start
		//D is inner end; E is outer start' Fis outer end
		int vertIndesA = (6 * sideNum) + 0;
		int vertIndesB = (6 * sideNum) + 1;
		int vertIndesC = (6 * sideNum) + 2;
		int vertIndesD = (6 * sideNum) + 3;
		int vertIndesE = (6 * sideNum) + 4;
		int vertIndesF = (6 * sideNum) + 5;

		verts[vertIndesA].m_position = innerEndPos;
		verts[vertIndesB].m_position = innerStartPos;
		verts[vertIndesC].m_position = outerStartPos;
		verts[vertIndesA].m_color = color;
		verts[vertIndesB].m_color = color;
		verts[vertIndesC].m_color = color;

		verts[vertIndesD].m_position = innerEndPos;
		verts[vertIndesE].m_position = outerStartPos;
		verts[vertIndesF].m_position = outerEndPos;
		verts[vertIndesD].m_color = color;
		verts[vertIndesE].m_color = color;
		verts[vertIndesF].m_color = color;
	}

	g_theRenderer->DrawVertexArray(NUM_VERTS, verts);
}

void DebugDrawLine(Vec2 const& startPoint, float DegreeTowardEndDirection, float length, float thickness, Rgba8 const& color)
{
	//float halfThickness = 0.5f * thickness;
	//Vec2 forward = Vec2(length * CosDegrees(DegreeTowardEndDirection), length * SinDegrees(DegreeTowardEndDirection));
	//Vec2 leftShort = forward.GetRotated90Degrees();
	//leftShort.SetLength(halfThickness);
	//Vec2 startLeft = startPoint + leftShort;
	//Vec2 starRight = startPoint - leftShort;
	//Vec2 endLeft = startPoint + forward + leftShort;
	//Vec2 endRight = startPoint + forward - leftShort;
	//constexpr int NUM_TRIS = 2;
	//constexpr int NUM_VERTS = 3 * NUM_TRIS;
	//Vertex_PCU verts[NUM_VERTS];

	//verts[0].m_position = Vec3(startLeft.x, startLeft.y, 0.f);
	//verts[1].m_position = Vec3(starRight.x, starRight.y, 0.f);
	//verts[2].m_position = Vec3(endRight.x, endRight.y, 0.f);
	//verts[3].m_position = Vec3(endRight.x, endRight.y, 0.f);
	//verts[4].m_position = Vec3(endLeft.x, endLeft.y, 0.f);
	//verts[5].m_position = Vec3(startLeft.x, startLeft.y, 0.f);

	//for (int vertIndex = 0; vertIndex < NUM_VERTS; vertIndex++) {
	//	verts[vertIndex].m_color = color;
	//}
	//g_theRenderer->DrawVertexArray(NUM_VERTS, verts);

	Vec2 startToEndPoint(length * CosDegrees(DegreeTowardEndDirection), length * SinDegrees(DegreeTowardEndDirection));
	DebugDrawLine(startPoint, startToEndPoint + startPoint, thickness, color);
}
void DebugDrawLine(Vec2 const& startPoint, Vec2 const& endPoint,float thickness, Rgba8 const& color) {

	constexpr int NUM_SIDES = 6;       
	constexpr int NUM_VERTS = NUM_SIDES;
	constexpr int vertNum = NUM_VERTS;
	Vertex_PCU verts[NUM_VERTS];
	float radius = 0.5f * thickness;
	Vec3 vecE (endPoint.x, endPoint.y, 0.f);
	Vec3 vecS(startPoint.x, startPoint.y, 0.f);
	Vec3 vecD(endPoint.x - startPoint.x, endPoint.y - startPoint.y, 0.f);
	Vec3 vecF = vecD.GetNormalized();
	Vec3 vecFS = vecF * radius;
	Vec3 vecLS = vecFS.GetRotatedAboutZDegrees(90.f);
	Vec3 vecEL = vecE + vecF + vecLS;
	Vec3 vecER = vecE + vecF - vecLS;
	Vec3 vecSL = vecS - vecF + vecLS;
	Vec3 vecSR = vecS - vecF - vecLS;
	verts[NUM_VERTS - 1].m_position = vecSL;
	verts[NUM_VERTS - 2].m_position = vecSR;
	verts[NUM_VERTS - 3].m_position = vecER;
	verts[NUM_VERTS - 4].m_position = vecER;
	verts[NUM_VERTS - 5].m_position = vecEL;
	verts[NUM_VERTS - 6].m_position = vecSL;
	for (int vertsNum = 0; vertsNum < NUM_VERTS; ++vertsNum)
	{
		verts[vertsNum].m_color = color;
	} 
	g_theRenderer->DrawVertexArray(NUM_VERTS, verts);
}