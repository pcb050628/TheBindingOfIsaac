#pragma once

struct Vtx
{
	Vec3 vPos;			// transform
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

struct tMaterial
{
	int iArr[4];
	float floatArr[4];
	Vec2  Vec2Arr[4];
	Vec4  Vec4Arr[4];
	Matrix matArr[4];
};
