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
	Matrix matWorldInv;
	Matrix matView;
	Matrix matViewInv;
	Matrix matProj;
	Matrix matProjInv;

	Matrix matWV;
	Matrix matWVP;
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