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

struct tMtrlData
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int	bTex[TEX_PARAM::END];

	int iPadding[2];
};
extern tMtrlData g_Material;

struct tAnimData
{
	Vec2 vLeftTop;
	Vec2 vSliceSize;
	Vec2 vOffset;
	Vec2 vBackGround;
	int  UseAnim2D;
	Vec3 vPadding;
};
extern tAnimData g_AnimData;

struct tGlobalData
{
	Vec2	g_RenderResolution;	// 렌더링 해상도
	float	g_dt;                 // Delta Time
	float	g_time;               // 누적 시간
	int		g_Light2DCount;         // 2D 광원 개수
	int		g_Light3DCount;         // 3D 광원 개수
	Vec2	globalpadding;
};
extern tGlobalData g_globalData;

struct tLightInfo
{
	Vec4 vColor;
	Vec4 vSpecular;
	Vec4 vAmbient;

	Vec3	vWorldPos;
	Vec3	vWorldDir;
	float	fRadius;	
	float	fAngle;		

	int		LightType;

	Vec3	vPadding;
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
	END
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	CROSS,
	CUBE,
	SPHERE,
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE	eShape;

	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRot;
	Matrix		matWorld;

	Vec3		vColor;
	float		fLifeTime;
	float		fDuration;
	bool		bDepthTest;
};

