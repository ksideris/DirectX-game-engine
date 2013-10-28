

#include "pch.h"
#include "DynamicObject.h"

bool DynamicObject::IsColliding(float2  otherPos )
{
	/*
	Rectangle collisionGeometry = Rectangle();
	collisionGeometry.Top = pos.y - (size.y / 2.0);
	collisionGeometry.Bottom = pos.y + (size.y / 2.0);
	collisionGeometry.Left = pos.x - (size.x / 2.0);
	collisionGeometry.Right = pos.x + (size.x / 2.0);

	Rectangle OtherCollisionGeometry = Rectangle();
	OtherCollisionGeometry.Top = OtherObject->pos.y - (OtherObject->size.y / 2.0);
	OtherCollisionGeometry.Bottom = OtherObject->pos.y + (OtherObject->size.y / 2.0);
	OtherCollisionGeometry.Left = OtherObject->pos.x - (OtherObject->size.x / 2.0);
	OtherCollisionGeometry.Right = OtherObject->pos.x + (OtherObject->size.x / 2.0);


	if (collisionGeometry.Bottom < OtherCollisionGeometry.Top) return(0);
	if (collisionGeometry.Top > OtherCollisionGeometry.Bottom) return(0);

	if (collisionGeometry.Right < OtherCollisionGeometry.Left) return(0);
	if (collisionGeometry.Left > OtherCollisionGeometry.Right) return(0);
	 */
	if (sqrt(pow(otherPos.x - pos.x, 2) + pow(otherPos.y - pos.y, 2)) < size.x )
		return 1;

	else 
		return 0;
	  
}
 