
#include "pch.h" 
#include "CollidingObject.h"
#include <limits>

float2 rotate_point(float2 p, float2 c, float angle)
{

	// translate point back to origin:
	p.x -= c.x;
	p.y -= c.y;

	// rotate point
	float xnew = p.x * cos(angle) - p.y * sin(angle);
	float ynew = p.x * sin(angle) + p.y * cos(angle);

	// translate point back:
	p.x = xnew + c.x;
	p.y = ynew + c.y;
	return p;
}
 

float3 getLine(std::pair<float2, float2> la)
{
	float2 a = la.first;
	float2 b = la.second;

	if (a.y == b.y)
		return float3(1, 0, b.y);
	else if (a.x == b.x)
		return float3(0, 1, b.x);
	else
	{
		float inc = (b.y - a.y) / float(b.x - a.x);
		return float3((-inc), 1, a.y - inc*a.x);
	}
}

float  minimum_distance(std::pair<float2, float2> lineSeg, float2 p)
{
	float2 A = lineSeg.first;
	float2 B = lineSeg.second;

	float l2 = pow(dist(A, B), 2);

	if (l2 == 0.0)
		return dist(p, A);

	float t = dot(p - A, B - A) / l2;

	if (t < 0.0)
		return dist(p, A);
	else if (t > 1.0)
		return dist(p, B);
	return dist(p, A + t * (B - A));
}


// Calculate the projection of a polygon on an axis
// and returns it as a [min, max] interval
void ProjectPolygon(float2 axis, CollisionGeometry polygon, float & min, float & max) {
	// To project a point on an axis use the dot product
	float dotProduct = dot(axis, polygon.at(0));

	min = dotProduct;
	max = dotProduct;

	for (auto point = polygon.begin(); point != polygon.end(); point++)
	{
		dotProduct = dot(axis, *point);
		if (dotProduct < min) {
			min = dotProduct;
		}
		else {
			if (dotProduct> max) {
				max = dotProduct;
			}
		}
	}
}

float IntervalDistance(float minA, float maxA, float minB, float maxB) {
	if (minA < minB) {
		return minB - maxA;
	}
	else {
		return minA - maxB;
	}
}

float2 getPolygonCenter(CollisionGeometry polygonA) {
	float2 mean = float2(0.f, 0.f);

	for (auto point = polygonA.begin(); point != polygonA.end(); point++)
		mean = mean + *point;

	return  mean *(1.f / polygonA.size());
}


bool PolygonCollision(CollisionGeometry polygonA,CollisionGeometry polygonB)//, float2 velocity) 
{
	PolygonCollisionResult* result = new PolygonCollisionResult();
	result->Intersect = true;
	result->WillIntersect = true;

	bool isIntersecting = true;
	int edgeCountA = polygonA.size();
	int edgeCountB = polygonB.size();

	float minIntervalDistance = std::numeric_limits<float>::infinity();
	float2 translationAxis;
	float2 edge;

	 
	// Loop through all the edges of both polygons
	for (int edgeIndex = 0; edgeIndex < edgeCountA + edgeCountB; edgeIndex++) {
		if (edgeIndex < edgeCountA) {

			if (edgeIndex == edgeCountA - 1)
				edge = float2(polygonA.at(0) - polygonA.at(edgeIndex));
			else
				edge = float2(polygonA.at(edgeIndex + 1) - polygonA.at(edgeIndex));

		}
		else {

			if (edgeIndex == edgeCountB + edgeCountA - 1)
				edge = float2(polygonB.at(edgeIndex - edgeCountA) - polygonB.at(  0));
			else
				edge = float2(polygonB.at(edgeIndex - edgeCountA + 1) - polygonB.at(edgeIndex - edgeCountA));

		}

		// ===== 1. Find if the polygons are currently intersecting =====

		// Find the axis perpendicular to the current edge

		float2 axis = norm(float2(-edge.y, edge.x));


		// Find the projection of the polygon on the current axis
		float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
		ProjectPolygon(axis, polygonA, minA, maxA);
		ProjectPolygon(axis, polygonB, minB, maxB);

		// Check if the polygon projections are currentlty intersecting
		if (IntervalDistance(minA, maxA, minB, maxB) > 0)\
			isIntersecting = false;

		//// ===== 2. Now find if the polygons *will* intersect =====

		//// Project the velocity on the current axis
		//float velocityProjection = dot(axis,velocity);

		//// Get the projection of polygon A during the movement
		//if (velocityProjection < 0) {
		//	minA += velocityProjection;
		//}
		//else {
		//	maxA += velocityProjection;
		//}

		//// Do the same test as above for the new projection
		//float intervalDistance = IntervalDistance(minA, maxA, minB, maxB);
		//if (intervalDistance > 0) result->WillIntersect = false;

		//// If the polygons are not intersecting and won't intersect, exit the loop
		//if (!result->Intersect && !result->WillIntersect) break;

		//// Check if the current interval distance is the minimum one. If so store
		//// the interval distance and the current distance.
		//// This will be used to calculate the minimum translation vector
		//intervalDistance = abs(intervalDistance);
		//if (intervalDistance < minIntervalDistance) {
		//	minIntervalDistance = intervalDistance;
		//	translationAxis = axis;

		//	float2 d = getPolygonCenter(polygonA) - getPolygonCenter(polygonB);
		//	if (dot(d,translationAxis) < 0)
		//		translationAxis = -translationAxis;
		//}
	}

	// The minimum translation vector
	// can be used to push the polygons appart.
	/*if (result->WillIntersect)
		result->MinimumTranslationVector =
		translationAxis * minIntervalDistance;*/

	return isIntersecting;
}


bool checkIntersection(std::pair<float2, float2> la, std::pair<float2, float2> lb, float &x, float &y)
{
	float3 lineA = getLine(la);
	float3 lineB = getLine(lb);
	float det = lineA.x*lineB.y - lineA.y*lineB.x;

	if (abs(det) < 1.0e-10)
	{
		if (lineA == lineB)
		{
			if (dist(la.first, la.second) <= dist(lb.first, lb.second))
			{
				y = (la.first.y + la.second.y) / 2.0f;
				x = (la.first.x + la.second.x) / 2.0f;
			}
			else
			{
				y = (lb.first.y + lb.second.y) / 2.0f;
				x = (lb.first.x + lb.second.x) / 2.0f;
			}
			return true;
		}
		return false;
	}
	else{

		y = (lineB.y*lineA.z - lineA.y*lineB.z) / det;
		x = (lineB.z*lineA.x - lineA.y*lineA.z) / det;
		float dist = minimum_distance(la, float2(x, y));
		float dist2 = minimum_distance(lb, float2(x, y));
		if (dist < 1.0e-6 || dist2 < 1.0e-6)
			return true;
		else
			return false;
	}
}
//simple collision detection
//
bool  checkCollision(CollisionGeometry ag, CollisionGeometry og)
{

	std::pair<float2, float2> la;
	std::pair<float2, float2> lb;

	for (auto selfpoint = ag.begin(); selfpoint != ag.end(); selfpoint++)
	{
		if (selfpoint == ag.end() - 1)
			la = std::pair<float2, float2>(*selfpoint, *ag.begin());
		else
			la = std::pair<float2, float2>(*selfpoint, *(selfpoint + 1));

		for (auto point = og.begin(); point != og.end(); point++)
		{
			if (point == og.end() - 1)
				lb = std::pair<float2, float2>(*point, *og.begin());
			else
				lb = std::pair<float2, float2>(*point, *(point + 1));

			float x, y;
			if (checkIntersection(la, lb, x, y))
				return true;

		}
	}
	return false;
}

void CollidingObject::scaleCollisionGeometry(float2 scale)
{

}
void CollidingObject::rotateCollisionGeometry(float  rot, float2 pos){

	for (auto selfpoint = polygon.begin(); selfpoint != polygon.end(); selfpoint++)
		*selfpoint = rotate_point(*selfpoint, pos, rot);
}
void CollidingObject::translateCollisionGeometry(float2 offset){

	for (auto selfpoint = polygon.begin(); selfpoint != polygon.end(); selfpoint++)
		*selfpoint = *selfpoint + offset;
}
void CollidingObject::setRectangleCollisionGeometry(float2 topleft, float2 bottomright){
	polygon.clear();
	polygon.push_back(topleft);
	polygon.push_back(float2(bottomright.x, topleft.y));
	polygon.push_back(bottomright);
	polygon.push_back(float2(topleft.x, bottomright.y));
}
void CollidingObject::setForwardTriangleCollisionGeometry(float2 topleft, float2 bottomright){
	polygon.clear();
	polygon.push_back(topleft);
	polygon.push_back(float2(bottomright.x, (bottomright.y + topleft.y) / 2.0f));
	polygon.push_back(float2(topleft.x, bottomright.y));
}
CollisionGeometry CollidingObject::getCollisionGeometry(){
	return polygon;
}

void CollidingObject::DebugDraw(BasicSprites::SpriteBatch^ m_spriteBatch, Microsoft::WRL::ComPtr<ID3D11Texture2D>  debug_texture)
{

	std::vector<float2> cg = getCollisionGeometry();

	for (auto corner = cg.begin(); corner != cg.end(); corner++)
		m_spriteBatch->Draw(
		debug_texture.Get(),
		*corner,
		PositionUnits::DIPs,
		float2(0.15f, 0.15f),
		SizeUnits::Normalized,
		float4(0.8f, 0.8f, 1.0f, 1.0f),
		0
		);

}