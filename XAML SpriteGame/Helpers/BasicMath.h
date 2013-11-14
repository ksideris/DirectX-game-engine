//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

// Common Constants

#define PI_F 3.1415927f

// Template Vector & Matrix Classes


template <class T> struct Vector2
{
    union
    {
        struct
        {
            T x;
            T y;
        };
        struct
        {
            T r;
            T g;
        };
        struct
        {
            T u;
            T v;
        };
    };

    T& operator[](unsigned int index)
    {
        return static_cast<T*>(this)[index];
    }

    Vector2(T _x = 0, T _y = 0) : x(_x), y(_y) { }
};

template <class T> struct Vector3
{
    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        struct
        {
            T r;
            T g;
            T b;
        };
        struct
        {
            T u;
            T v;
            T w;
        };
    };

    T& operator[](unsigned int index)
    {
        return static_cast<T*>(this)[index];
    }

    Vector3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) { }
};

template <class T> struct Vector4
{
    union
    {
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };
        struct
        {
            T r;
            T g;
            T b;
            T a;
        };
    };

    T& operator[](unsigned int index)
    {
        return static_cast<T*>(this)[index];
    }

    Vector4(T _x = 0, T _y = 0, T _z = 0, T _w = 0) : x(_x), y(_y), z(_z), w(_w) { }
};

// Template Vector Operators
template <class T>
float len(Vector2<T> a)
{
	return sqrt(pow(a.x  , 2.0f)+ pow(a.y  , 2.0f));
}

template <class T>
Vector2<T> norm(Vector2<T> a)
{
	float _len = len(a);
	return Vector2<T>( a.x   / _len,  a.y   / _len);
}

template <class T> 
float dist(Vector2<T> a, Vector2<T> b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(b.y - b.y, 2));
}

template <class T>
float dot(Vector2<T> a, Vector2<T> b)
{
	return  a.x * b.x+ a.y * b.y ;
}

template <class T>
Vector2<T> operator-(Vector2<T> a, Vector2<T> b)
{
    return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <class T>
Vector2<T> operator-(Vector2<T> a)
{
    return Vector2<T>( -a.x, -a.y);
}

template <class T>
Vector3<T> operator-(Vector3<T> a, Vector3<T> b)
{
    return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <class T>
Vector3<T> operator-(Vector3<T> a)
{
    return Vector3<T>( -a.x, -a.y, -a.z);
}
template <class T>
bool operator==(Vector3<T> a, Vector3<T> b)
{
	return    a.x == b.x && a.y == b.y && a.z == b.z;
}


template <class T>
Vector4<T> operator-(Vector4<T> a, Vector4<T> b)
{
    return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <class T>
Vector4<T> operator-(Vector4<T> a)
{
    return Vector4<T>( -a.x, -a.y, -a.z, -a.w);
}

template <class T>
Vector2<T> operator+(Vector2<T> a, Vector2<T> b)
{
    return Vector2<T>(a.x + b.x, a.y + b.y);
}

template <class T>
Vector3<T> operator+(Vector3<T> a, Vector3<T> b)
{
    return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <class T>
Vector4<T> operator+(Vector4<T> a, Vector4<T> b)
{
    return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <class T>
Vector2<T> operator*(Vector2<T> a, T s)
{
    return Vector2<T>(a.x * s, a.y * s);
}

template <class T>
Vector2<T> operator*(T s, Vector2<T> a)
{
    return a * s;
}

template <class T>
Vector2<T> operator*(Vector2<T> a, Vector2<T> b)
{
    return Vector2<T>(a.x * b.x, a.y * b.y);
}

template <class T>
Vector2<T> operator/(Vector2<T> a, T s)
{
    return Vector2<T>(a.x / s, a.y / s);
}

template <class T>
Vector3<T> operator*(Vector3<T> a, T s)
{
    return Vector3<T>(a.x * s, a.y * s, a.z * s);
}

template <class T>
Vector3<T> operator*(T s, Vector3<T> a)
{
    return a * s;
}

template <class T>
Vector3<T> operator*(Vector3<T> a, Vector3<T> b)
{
    return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <class T>
Vector3<T> operator/(Vector3<T> a, T s)
{
    return Vector3<T>(a.x / s, a.y / s, a.z / s);
}

template <class T>
Vector4<T> operator*(Vector4<T> a, T s)
{
    return Vector4<T>(a.x * s, a.y * s, a.z * s, a.w * s);
}

template <class T>
Vector4<T> operator*(T s, Vector4<T> a)
{
    return a * s;
}

template <class T>
Vector4<T> operator*(Vector4<T> a, Vector4<T> b)
{
    return Vector4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <class T>
Vector4<T> operator/(Vector4<T> a, T s)
{
    return Vector4<T>(a.x / s, a.y / s, a.z / s, a.w / s);
}

// Common HLSL-compatible vector typedefs

typedef unsigned int uint;

typedef Vector2<float> float2;
typedef Vector3<float> float3;
typedef Vector4<float> float4;


inline float RandFloat(float min, float max)
{
	return (static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX)) * (max - min) + min;
};

inline float CalculateRadians(float2 velocity)
{
	float hypothenuse = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	float sine = velocity.y / hypothenuse;
	float angle = asin(sine);
	return angle;
};

inline float CalculateRadiansFromAngle(float angle)
{
	return angle * PI_F / 180.0f;
};

inline float ConvertDipsToPixels(float dips, float dpi)
{
	static const float dipsPerInch = 96.0f;
	return floor(dips * dpi / dipsPerInch + 0.5f);
};


 

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}