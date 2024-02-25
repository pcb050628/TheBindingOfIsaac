#include "pch.h"
#include "ResourceManager.h"

#include "Chapter.h"

#include "Mesh.h"
#include "GraphicsShader.h"
#include "ChapterManager.h"

#include "fstream"
#include "istream"
#include "filesystem"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < (UINT)RESOURCE_TYPE::END; i++)
	{
		for (auto iter : m_Resources[i])
		{
			delete iter.second;
		}
		m_Resources[i].clear();
	}
}

void ResourceManager::Init()
{
	CreateDefaultMesh();
	//CreateDefaultShader();
	//LoadAnim();
	//m_ObjFile.push_back(L"test_rock.txt");
	std::wstring path = GetContentPath() + L"Resource\\";
	LoadAllContent(path);
	//CreateDefaultMaterial();
}

void ResourceManager::LoadAllContent(const std::wstring& _path)
{
	if (filesystem::exists(_path))
	{
		if (filesystem::is_directory(_path))
		{
			for (const auto& entry : filesystem::directory_iterator(_path))
			{
				LoadAllContent(entry.path());
			}
		}
		else if (filesystem::is_regular_file(_path))
		{
			LoadResource(_path);
		}
	}
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
	GraphicsShader* gs = nullptr;
	
	gs = new GraphicsShader;
	gs->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	gs->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");
	gs->m_RSType = RS_TYPE::CULL_NONE;
	gs->m_Domain = SHADER_DOMAIN::DOMAIN_MASKED;
	gs->SetResourceName(L"default_Shader");
	gs->Save();
	delete gs;

	gs = new GraphicsShader;
	gs->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	gs->CreatePixelShader(L"Shader\\std2d.fx", "PS_UI2D");
	gs->m_RSType = RS_TYPE::CULL_NONE;
	gs->m_BSType = BS_TYPE::ALPHABLEND;
	gs->m_Domain = SHADER_DOMAIN::DOMAIN_MASKED;
	gs->SetResourceName(L"ui_Shader");
	gs->Save();
	delete gs;
	
	gs = new GraphicsShader;
	gs->CreateVertexShader(L"Shader\\debug.fx", "VS_DebugShape");
	gs->CreatePixelShader(L"Shader\\debug.fx", "PS_DebugShape");
	gs->m_Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	gs->m_RSType = RS_TYPE::CULL_NONE;
	gs->m_DSSType = DSS_TYPE::NO_TEST_NO_WRITE;
	gs->m_Domain = SHADER_DOMAIN::DOMAIN_DEBUG;
	gs->SetResourceName(L"debug_Shader");
	gs->Save();
	delete gs;
}

void ResourceManager::CreateDefaultMaterial()
{
	Material* mtrl = new Material;
	mtrl->SetResourceName(L"default_Material");
	mtrl->SetShader(Find<GraphicsShader>(L"default_Shader"));
	mtrl->Save();
	delete mtrl;

	mtrl = new Material;
	mtrl->SetResourceName(L"debug_Material");
	mtrl->SetShader(Find<GraphicsShader>(L"debug_Shader"));
	mtrl->Save();
	delete mtrl;
	
	mtrl = new Material;
	mtrl->SetResourceName(L"ui_Material");
	mtrl->SetShader(Find<GraphicsShader>(L"ui_Shader"));
	mtrl->Save();
	delete mtrl;
}

void ResourceManager::LoadAnim()
{
	Load<Anim>(L"Link_RightAnim_test.txt");
}

int ResourceManager::LoadResource(const std::wstring& _path)
{
	wchar_t szExt[20] = {};
	_wsplitpath_s(_path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	wchar_t szName[100] = {};
	_wsplitpath_s(_path.c_str(), nullptr, 0, nullptr, 0, szName, 100, nullptr, 0);

	std::wstring ext(szExt);

	Resource* r = nullptr;

	if (L".anim" == ext)
	{
		r = Load<Anim>(_path, true);
		if (nullptr == r) return E_FAIL; else return S_OK;
	}
	else if (L".gobj" == ext)
	{
		std::wstring file(szName);
		file += szExt;
		m_ObjFile.push_back(file);
		return S_OK;
	}
	else if (L".mtrl" == ext)
	{
		r = Load<Material>(_path, true);
		if (nullptr == r) return E_FAIL; else return S_OK; 
	}
	else if (L".gs" == ext)
	{
		r = Load<GraphicsShader>(_path, true);
		if (nullptr == r) return E_FAIL; else return S_OK; 
	}
	else if (L".room" == ext)
	{
		std::wstring file(szName);
		file += szExt;
		m_RoomFile.push_back(file);
		return S_OK;
	}
	else
	{
		r = Load<Texture>(_path, true);
		if (nullptr == r) return E_FAIL; else return S_OK; 
	}

	return E_FAIL;
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
	if (_type == RESOURCE_TYPE::GAMEOBJECT)
	{
		for (const std::wstring& file : m_ObjFile)
		{
			_vecStr.push_back(ToString(file));
		}
	}
	else
	{
		for (const auto asset : m_Resources[(UINT)_type])
		{
			_vecStr.push_back(ToString(asset.first));
		}
	}
}
