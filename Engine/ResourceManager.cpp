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
	Load<Texture>(L"Rocks.png");
	Load<Texture>(L"Link.png");
	Load<Anim>(L"Link_RightAnim_test.txt");
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
	pMesh->SetResourceName(L"RectMesh");
	AddResource(pMesh->GetResourceName(), pMesh);

	// Topology LineStrip 용도	
	//   0(Red)-- 1(Blue)	     
	//    |       |	     
	//   3(G)---- 2(Magenta)   
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;	arrIdx[3] = 3; 	arrIdx[4] = 0;

	pMesh = new Mesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetResourceName(L"RectMesh_Debug");
	AddResource(pMesh->GetResourceName(), pMesh);

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
	pMesh->SetResourceName(L"CircleMesh");
	AddResource(pMesh->GetResourceName(), pMesh);

	// CircleMesh_Debug
	vecIdx.clear();
	for (int i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new Mesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetResourceName(L"CircleMesh_Debug");
	AddResource(pMesh->GetResourceName(), pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void ResourceManager::CreateDefaultShader()
{
	Load<GraphicsShader>(L"default_Shader.txt");
	Load<GraphicsShader>(L"debug_Shader.txt");
	Load<GraphicsShader>(L"ui_Shader.txt");
}

void ResourceManager::CreateDefaultMaterial()
{
	Load<Material>(L"default_Material.txt");
	Load<Material>(L"debug_Material.txt");
	Load<Material>(L"ui_Material.txt");
}

Texture* ResourceManager::CreateTexture(const std::wstring& _strKey, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlags, D3D11_USAGE _usage)
{
	Texture* pTex = new Texture();

	if (FAILED(pTex->Create(_width, _height, _format, _bindFlags, _usage)))
	{
		MessageBoxW(nullptr, L"Texture 생성 실패", L"리소스 생성 실패", MB_OK);
		return nullptr;
	}

	if (FAILED(AddResource(_strKey, pTex)))
	{
		return nullptr;
	}

	return pTex;
}

Texture* ResourceManager::CreateTexture(const std::wstring& _strKey, Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex2D)
{
	Texture* pTex = new Texture();

	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBoxW(nullptr, L"Texture 생성 실패", L"리소스 생성 실패", MB_OK);
		return nullptr;
	}

	if (FAILED(AddResource(_strKey, pTex)))
	{
		return nullptr;
	}

	return pTex;
}

void ResourceManager::GetAssetName(RESOURCE_TYPE _type, std::vector<std::string>& _vecStr)
{
	for (const auto asset : m_Resources[(UINT)_type])
	{
		_vecStr.push_back(ToString(asset.first));
	}
}
