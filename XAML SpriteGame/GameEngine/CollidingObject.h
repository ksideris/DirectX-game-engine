#pragma once
#include <stdlib.h> 
#include "Helpers\BasicMath.h" 

typedef std::vector<float2> CollisionGeometry;

// Structure that stores the results of the PolygonCollision function
 struct PolygonCollisionResult {
	// Are the polygons going to intersect forward in time?
	 bool WillIntersect;
	// Are the polygons currently intersecting?
	 bool Intersect;
	// The translation to apply to the first polygon to push the polygons apart.
	 float2 MinimumTranslationVector;
 };


class CollidingObject
{
protected:
	CollisionGeometry polygon;
	void scaleCollisionGeometry(float2 scale);
	void translateCollisionGeometry(float2 offset);

public:

	void rotateCollisionGeometry(float rot, float2 pos);
	void setRectangleCollisionGeometry(float2 topleft, float2 bottomright);
	CollisionGeometry getCollisionGeometry();
	void DebugDraw(BasicSprites::SpriteBatch^ m_spriteBatch, Microsoft::WRL::ComPtr<ID3D11Texture2D>  debug_texture);
	void setForwardTriangleCollisionGeometry(float2 topleft, float2 bottomright);
	
};

 
bool PolygonCollision(CollisionGeometry polygonA, CollisionGeometry polygonB);
 
 

float2 rotate_point(float2 p, float2 c, float angle);
 