#pragma once

#ifndef _H_VECTORS
#define _H_VECTORS

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Vector2
{
	float x;
	float y;
};

struct AxisAlignedBox {
	Vector3 min;
	Vector3 max;
};

struct Plane {
	short height[3][3];
};


static Vector2 Vec3ToVec2(Vector3 input_vector) {
	Vector2 vec2;
	vec2.x = input_vector.x;
	vec2.y = input_vector.y;
	return vec2;
}

#endif
