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
	GLOBAL_DATA,
	ANIMATION,

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