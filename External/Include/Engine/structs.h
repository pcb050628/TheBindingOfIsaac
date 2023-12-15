#pragma once

struct Vtx
{
	Vec3 vPos;	// transform
	Vec4 vColor;		// color
	Vec2 vUV;			// texcoord
};

struct tTransform
{
	Vec4	vWorldPos;  
	Vec4    vWorldScale; 
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