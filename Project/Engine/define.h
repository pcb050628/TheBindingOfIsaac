#pragma once

#define SINGLETON(type)\
				private:\
					type();\
					type(const type& _origin) = delete;\
					~type();\
				public:\
					static type* GetInst()\
					{\
						static type Inst;\
						return &Inst;\
					}\
					type* operator = (const type& _other) = delete;

#define PI 3.14159265359

#define RENDER_RESOLUTION Device::GetInst()->GetRenderResolution()

enum DIRECTION
{
	LeftDir,
	RightDir,
	TopDir,
	BottomDir,
};

enum class LAYER_TYPE
{
	Background,
	Light,
	Object,
	Monster,
	Player,
	Pickup,
	Item,
	UI,
	Camera,
	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,	

	COLLIDER2D,	
	COLLIDER3D, 

	ANIMATOR2D,	
	ANIMATOR3D,	

	LIGHT2D,	
	LIGHT3D,	

	CAMERA,		

	// Render Component
	MESHRENDERER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,

	SCRIPT,
};

enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	ANIMATION2D,
	GLOBAL_DATA,

	END,
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,

	END,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,

	END,
};

enum class DSS_TYPE
{
	LESS,
	LESS_EQUAL,
	GRATER,
	GRATER_EQUAL,

	NO_TEST,
	NO_WRITE,
	NO_TEST_NO_WRITE,

	END,
};

enum class BS_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONE_ONE,

	END,
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

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAUQUE,
	DOMAIN_MASKED,
	DOMAIN_TRANSPARENT,
	DOMAIN_POSTPROCESS,

	DOMAIN_DEBUG,
};