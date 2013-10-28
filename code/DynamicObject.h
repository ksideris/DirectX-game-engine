

class Rectangle
{
public:
	float Left;
	float Right;
	float Top;
	float Bottom;
};

#pragma once

#include "DirectXBase.h"
#include "BasicSprites.h"

class DynamicObject
{

public:
	float2 pos;
	float2 vel;
	float rot;
	float rotVel;
	float scale;
	float2 accel;
	 
	float2 size; 

	Microsoft::WRL::ComPtr<ID3D11Texture2D> text;

	bool IsColliding(float2  otherPos );

};
 