#pragma once

#include <vector>
#include <queue>
#include <map>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "wrl.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXTex/DirectXTex.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\External\\Library\\DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "..\\..\\External\\Library\\DirectXTex\\DirectXTex")
#endif

#include <DirectX/SimpleMath.h>
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#include "define.h"
#include "structs.h"
#include "functions.h"
