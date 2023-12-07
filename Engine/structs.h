#pragma once

struct Vtx
{
	Vec3 vTransform;	// transform
	Vec4 vColor;		// color
	Vec2 vUV;			// texcoord
};

enum class GOBJ_TYPE
{
	GOBJ,
	MONSTER,
	PLAYER,
	OBJECT,
	TRAP,
	DOOR,
	TRAPDOOR,
};