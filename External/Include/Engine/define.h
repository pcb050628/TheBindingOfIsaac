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
	TRANSFORM,	// ������Ʈ�� ��ġ,ũ��,ȸ��

	COLLIDER2D,	// 2���� �浹ü
	COLLIDER3D, // 3���� �浹ü

	ANIMATOR2D,	// ��������Ʈ Animation
	ANIMATOR3D,	// Bone Skinning Animation

	LIGHT2D,	// 2���� ����
	LIGHT3D,	// 3���� ����

	CAMERA,		// ī�޶� ���

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


// ������� ����
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