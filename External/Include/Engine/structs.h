#pragma once

struct Vtx
{
	Vec3 vPos;	// transform
	Vec4 vColor;		// color
	Vec2 vUV;			// texcoord
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
};
extern tTransform g_Transform;

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