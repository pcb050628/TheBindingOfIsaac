#include "pch.h"

tTransform g_Transform = {};
tMtrlData g_Material = {};
tAnimData g_AnimData = {};
tGlobalData g_globalData = {};

std::string LAYER_TYPE_STRING[(UINT)LAYER_TYPE::END] =
{
	"Background",
	"Light",
	"Object",
	"Monster",
	"Player",
	"Pickup",
	"Item",
	"UI",
	"Camera",
};

std::string COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END] =
{
	"TRANSFORM",

	"COLLIDER2D",
	"COLLIDER3D",

	"ANIMATOR2D",
	"ANIMATOR3D",

	"LIGHT2D",
	"LIGHT3D",

	"CAMERA",

	"MESHRENDERER",
	"TILEMAP",
	"PARTICLESYSTEM",

	"SKYBOX",
	"DECAL",
	"LANDSCAPE",
};