#include "pch.h"
#include "ResourceManager.h"

#include "Chapter.h"

#include "Mesh.h"
#include "GraphicsShader.h"
#include "ChapterManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Init()
{
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	Load<Texture>(L"Resource\\Image\\Rocks.png");
	//Load<Anim>(L"Resource\\Anim\\test_anim.txt");
}

void ResourceManager::CreateDefaultMesh()
{
	Mesh* pMesh = nullptr;

	// 전역변수에 삼각형 위치 설정
	//   0(Red)-- 1(Blue)	     
	//    |   \   |	     
	//   3(G)---- 2(Magenta)  
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// 인덱스
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;

	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	pMesh = new Mesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddResource(L"RectMesh", pMesh);

	// Topology LineStrip 용도	
	//   0(Red)-- 1(Blue)	     
	//    |       |	     
	//   3(G)---- 2(Magenta)   
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;	arrIdx[3] = 3; 	arrIdx[4] = 0;

	pMesh = new Mesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddResource(L"RectMesh_Debug", pMesh);

	// =================
	// CircleMesh 만들기
	// =================
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// 중심 점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT iSlice = 40;
	float fTheta = 0.f;
	float fRadius = 0.5f;

	for (UINT i = 0; i <= iSlice; ++i)
	{
		fTheta = (DirectX::XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new Mesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddResource(L"CircleMesh", pMesh);

	// CircleMesh_Debug
	vecIdx.clear();
	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new Mesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddResource(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void ResourceManager::CreateDefaultShader()
{
	// 이 처럼 불러오는 부분들중 기본 리소스가 아닌 특정 부분에서만 사용하는 리소스라면 아래의 LoadAllResource 함수를 통해서 가져올것
	GraphicsShader* pShader = nullptr;

	pShader = new GraphicsShader;
	Load<GraphicsShader>(L"Resource\\Shader\\Graphics\\default_Shader.txt");

	pShader = new GraphicsShader;
	Load<GraphicsShader>(L"Resource\\Shader\\Graphics\\debug_Shader.txt");
}

void ResourceManager::CreateDefaultMaterial()
{
	Material* pMaterial = nullptr;

	pMaterial = new Material;
	Load<Material>(L"Resource\\Material\\default_Material.txt");

	pMaterial = new Material;
	Load<Material>(L"Resource\\Material\\debug_Material.txt");
}

void ResourceManager::LoadAllResource(CHAPTERLEVEL _level) // Load All chapter resource , 순서는 Shader -> Image -> Material 순으로 로드
{
	//filesystem::path filePath = GetContentPath();
	//std::wifstream fileStream(filePath);
	//
	//wchar_t szName[20] = {};
	//_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);
	//
	//m_ResourceName = szName;
	//m_ResourcePath = _strFilePath;
}
