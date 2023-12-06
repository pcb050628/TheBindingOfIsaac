#include "pch.h"
#include "RenderManager.h"
#include "Engine.h"
#include "assert.h"

RenderManager::RenderManager()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRTView(nullptr)
	, m_bIsDrawing(false)
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::Init(HWND _hwnd, Vec2 _resolution)
{
	m_vResolution = _resolution;

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

		hResult = baseDevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_pDevice);
		assert(SUCCEEDED(hResult));
		baseDevice->Release();

		hResult = baseContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_pContext);
		assert(SUCCEEDED(hResult));
		baseContext->Release();
	}

	{ // SwapChain
		IDXGIFactory2* factory;
		{
			IDXGIDevice1* dxgiDevice;
			HRESULT hResult = m_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&dxgiDevice);
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
		swapChainDesc.Width = (UINT)m_vResolution.x;
		swapChainDesc.Height = (UINT)m_vResolution.y;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = 0;

		HRESULT hResult = factory->CreateSwapChainForHwnd(m_pDevice.Get(), _hwnd, &swapChainDesc, 0, 0, &m_pSwapChain);
		if (FAILED(hResult))
			return;
		factory->Release();
	}

	{ // Render Target View
		HRESULT hResult = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pRTTex);
		assert(SUCCEEDED(hResult));

		hResult = m_pDevice->CreateRenderTargetView(m_pRTTex.Get(), 0, m_pRTView.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}

	{ // DepthStencilTex, DepthStencilView
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		desc.Width = (UINT)m_vResolution.x;
		desc.Height = (UINT)m_vResolution.y;

		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.MipLevels = 1;
		desc.MiscFlags = 0;

		desc.ArraySize = 1;

		m_pDevice->CreateTexture2D(&desc, nullptr, m_pDSTex.GetAddressOf());
		m_pDevice->CreateDepthStencilView(m_pDSTex.Get(), nullptr, m_pDSView.GetAddressOf());
	}

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_vResolution.x;
	viewport.Height = m_vResolution.y;
	viewport.MaxDepth = 1.f;
	viewport.MinDepth = 0.f;

	m_pContext->RSSetViewports(1, &viewport);

	m_pContext->OMSetRenderTargets(1, m_pRTView.GetAddressOf(), m_pDSView.Get());

	m_vClearColor = Vec4(0.1f, 0.1f, 0.1f, 1.f);
}

void RenderManager::DrawStart()
{
	FLOAT color[4];
	color[0] = m_vClearColor.x;
	color[1] = m_vClearColor.y;
	color[2] = m_vClearColor.z;
	color[3] = m_vClearColor.w;

	m_pContext->ClearRenderTargetView(m_pRTView.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDSView.Get(), 0, 1, 1);
}
