#include "pch.h"
#include "RenderManager.h"
#include "Engine.h"
#include "math.h"
#include "assert.h"
#pragma comment(lib, "d3d11.lib")

RenderManager::RenderManager()
	: mp_Device(nullptr)
	, mp_Context(nullptr)
	, mp_SwapChain(nullptr)
	, mp_RenderTargetView(nullptr)
	, mp_SpriteBatch(nullptr)
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::Init(HWND _hwnd)
{
	{ // device initialize
		ID3D11Device*			baseDevice;
		ID3D11DeviceContext*	baseContext;

		D3D_FEATURE_LEVEL featureLevel[2] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_11_1
		};

		UINT creationFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		#if defined(DEBUG_BUILD)
			creationFlag |= D3D11_CREATE_DEVICE_DEBUG
		#endif // DEBUG

		HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0
			, creationFlag, featureLevel, ARRAYSIZE(featureLevel)
			, D3D11_SDK_VERSION, &baseDevice
			, 0, &baseContext);

		if (FAILED(hResult))
		{
			return;
		}

		hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&mp_Device);
		assert(SUCCEEDED(hResult));
		baseDevice->Release();

		hResult = baseContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&mp_Context);
		assert(SUCCEEDED(hResult));
		baseContext->Release();
	}

	{ // SwapChain
		IDXGIFactory2* factory;
		{
			IDXGIDevice1* dxgiDevice;
			HRESULT hResult = mp_Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
			if (FAILED(hResult))
			{
				return;
			}

			IDXGIAdapter* dxgiAdapter;
			hResult = dxgiDevice->GetAdapter(&dxgiAdapter);
			if (FAILED(hResult))
				return;
			dxgiDevice->Release();

			DXGI_ADAPTER_DESC adapterDesc;
			dxgiAdapter->GetDesc(&adapterDesc);
			OutputDebugStringA("Graphics Device : ");
			OutputDebugStringW(adapterDesc.Description);

			hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&factory);
			assert(SUCCEEDED(hResult));
			dxgiAdapter->Release();
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = 0;
		swapChainDesc.Height = 0;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = 0;

		HRESULT hResult = factory->CreateSwapChainForHwnd(mp_Device.Get(), _hwnd, &swapChainDesc, 0, 0, &mp_SwapChain);
		if (FAILED(hResult))
			return;
		factory->Release();
	}

	{ // Render Target View
		ID3D11Texture2D* d3d11FrameBuffer;
		HRESULT hResult = mp_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11FrameBuffer);
		assert(SUCCEEDED(hResult));

		hResult = mp_Device->CreateRenderTargetView(d3d11FrameBuffer, 0, mp_RenderTargetView.GetAddressOf());
		assert(SUCCEEDED(hResult));
		d3d11FrameBuffer->Release();
	}

	// SprtieBatch
	mp_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(mp_Context.Get());
	mp_SpriteFont = std::make_unique<DirectX::SpriteFont>(mp_Device.Get(), L"font file name");
	isDrawing = false;
}

void RenderManager::Update()
{
}

void RenderManager::TextureRender(ID3D11ShaderResourceView* _srv, Vec2 _pos, RECT& _iSection)
{
	if (!isDrawing)
	{
		return;
	}

	RECT dPos = { _pos.x, _pos.y, _pos.x + 10, _pos.y + 10 };

	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(_pos.x, _pos.y);
	DirectX::FXMVECTOR positionVector = DirectX::XMLoadFloat2(&position);

	//DirectX::XMFLOAT2 scale = DirectX::XMFLOAT2(1.0f, 1.0f); // 스케일 설정
	//DirectX::FXMVECTOR scaleVector = DirectX::XMLoadFloat2(&scale);

	mp_SpriteBatch->Draw(_srv, positionVector, &_iSection);
}

void RenderManager::FontRender(std::wstring _wstring, Vec2 _pos, DirectX::XMVECTORF32 _color)
{
	if (!isDrawing)
	{
		return;
	}
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(_pos.x, _pos.y);
	mp_SpriteFont->DrawString(mp_SpriteBatch.get(), _wstring.c_str(), position, _color);
}

void RenderManager::PrepareDraw()
{
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(Engine::GetInst()->GetResolution().right);
	viewport.Height = static_cast<float>(Engine::GetInst()->GetResolution().bottom);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	mp_Context->RSSetViewports(1, &viewport);
}

