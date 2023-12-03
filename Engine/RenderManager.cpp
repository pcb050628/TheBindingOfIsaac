#include "pch.h"
#include "RenderManager.h"
#include "Engine.h"
#include "assert.h"

RenderManager::RenderManager()
	: mp_Device(nullptr)
	, mp_Context(nullptr)
	, mp_SwapChain(nullptr)
	, mp_RTView(nullptr)
	, mp_SpriteBatch(nullptr)
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::Init(HWND _hwnd, Vec2 _resolution)
{
	m_Resolution = _resolution;

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
		swapChainDesc.Width = m_Resolution.x;
		swapChainDesc.Height = m_Resolution.y;
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
		HRESULT hResult = mp_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&mp_RTTex);
		assert(SUCCEEDED(hResult));

		hResult = mp_Device->CreateRenderTargetView(mp_RTTex.Get(), 0, mp_RTView.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}

	{ // DepthStencilTex, DepthStencilView
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		desc.Width = m_Resolution.x;
		desc.Height = m_Resolution.y;

		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.MipLevels = 1;
		desc.MiscFlags = 0;

		desc.ArraySize = 1;

		mp_Device->CreateTexture2D(&desc, nullptr, mp_DSTex.GetAddressOf());
		mp_Device->CreateDepthStencilView(mp_DSTex.Get(), nullptr, mp_DSView.GetAddressOf());

		mp_Context->OMSetRenderTargets(1, mp_RTView.GetAddressOf(), mp_DSView.Get());
	}

	// SprtieBatch
	mp_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(mp_Context.Get());
	//mp_SpriteFont = std::make_unique<DirectX::SpriteFont>(mp_Device.Get(), L"font file name");
	isDrawing = false;

	m_ClearColor = Vec4(0.1f, 0.1f, 0.1f, 1.f);
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

	float CColor[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w};

	mp_Context->ClearRenderTargetView(mp_RTView.Get(), CColor);
}

