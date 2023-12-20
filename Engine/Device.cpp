#include "pch.h"
#include "Device.h"
#include "Engine.h"
#include "assert.h"

#include "ConstantBuffer.h"

Device::Device()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRTView(nullptr)
	, m_arrConstantBuffer()
{

}

Device::~Device()
{

}

int Device::Init(HWND _hwnd, Vec2 _resolution)
{
	m_vResolution = _resolution;
	m_hRenderWnd = _hwnd;

	{ // device initialize
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		UINT creationFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		#if defined(DEBUG_BUILD)
			creationFlag |= D3D11_CREATE_DEVICE_DEBUG
		#endif // DEBUG

		HRESULT hResult = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0
			, creationFlag, &featureLevel, 1, D3D11_SDK_VERSION
			, m_pDevice.GetAddressOf(), 0, m_pContext.GetAddressOf());

		if (FAILED(hResult))
		{
			return E_FAIL;
		}
	}

	if(FAILED(CreateSwapChain()))
	{
		MessageBoxW(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateRenderTargetView()))
	{
		MessageBoxW(nullptr, L"Render Target View 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilView()))
	{
		MessageBoxW(nullptr, L"Depth Stencil View 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateRasterizerState()))
	{
		MessageBoxW(nullptr, L"Rasterizer State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBoxW(nullptr, L"Depth Stencil State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateBlendState()))
	{
		MessageBoxW(nullptr, L"Blend State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	{ // 뷰포트 설정
		D3D11_VIEWPORT viewport = {};

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = m_vResolution.x;
		viewport.Height = m_vResolution.y;
		viewport.MaxDepth = 1.f;
		viewport.MinDepth = 0.f;

		m_pContext->RSSetViewports(1, &viewport);
	}

	m_pContext->OMSetRenderTargets(1, m_pRTView.GetAddressOf(), m_pDSView.Get());

	m_vClearColor = Vec4(0.1f, 0.1f, 0.1f, 1.f);
}

void Device::DrawStart()
{
	FLOAT color[4];
	color[0] = m_vClearColor.x;
	color[1] = m_vClearColor.y;
	color[2] = m_vClearColor.z;
	color[3] = m_vClearColor.w;

	m_pContext->ClearRenderTargetView(m_pRTView.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDSView.Get(), 0, 1, 1);
}

int Device::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Width = (UINT)m_vResolution.x;
	swapChainDesc.BufferDesc.Height = (UINT)m_vResolution.y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.Flags = 0;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = true;
	swapChainDesc.OutputWindow = m_hRenderWnd;

	Microsoft::WRL::ComPtr<IDXGIDevice> pIdxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;


	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
	pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	HRESULT hResult = pFactory->CreateSwapChain(m_pDevice.Get(), &swapChainDesc, m_pSwapChain.GetAddressOf());
	if (FAILED(hResult))
		return E_FAIL;

	return S_OK;
}

int Device::CreateRenderTargetView()
{
	HRESULT hResult = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pRTTex); 
	if (FAILED(hResult)) 
		return E_FAIL;
	hResult = m_pDevice->CreateRenderTargetView(m_pRTTex.Get(), 0, m_pRTView.GetAddressOf()); 
	if (FAILED(hResult))
		return E_FAIL;

	return S_OK;
}

int Device::CreateDepthStencilView()
{
	HRESULT hResult = S_OK;

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

	hResult = m_pDevice->CreateTexture2D(&desc, nullptr, m_pDSTex.GetAddressOf());
	if (FAILED(hResult))
		return E_FAIL;

	hResult = m_pDevice->CreateDepthStencilView(m_pDSTex.Get(), nullptr, m_pDSView.GetAddressOf());
	if (FAILED(hResult))
		return E_FAIL;

	return S_OK;
}

int Device::CreateConstBuffer()
{
	m_arrConstantBuffer[(UINT)CB_TYPE::TRANSFORM] = new ConstantBuffer(CB_TYPE::TRANSFORM);
	m_arrConstantBuffer[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	return S_OK;
}

int Device::CreateRasterizerState()
{
	// CULL_BACK
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr; // 기본 설정이 cull_back 이기 때문에 nullptr(만들지 않아도)로 해도 된다

	D3D11_RASTERIZER_DESC desc = {};
	HRESULT hResult = S_OK;

	{ // CULL_FRONT
		desc.CullMode = D3D11_CULL_FRONT;
		desc.FillMode = D3D11_FILL_SOLID;

		hResult = m_pDevice->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
		if (FAILED(hResult)) 
			return E_FAIL;
	}

	{ // CULL_NONE
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;

		hResult = m_pDevice->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	{ // WIRE_FRAME
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;

		hResult = m_pDevice->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	return S_OK;
}

int Device::CreateDepthStencilState()
{
	m_arrDSS[(UINT)DSS_TYPE::LESS] = nullptr; // 기본 설정이 LESS 이기 때문에 nullptr(만들지 않아도)로 해도 된다

	D3D11_DEPTH_STENCIL_DESC desc = {};
	HRESULT hResult = S_OK;

	{ // LESS_EQUAL
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		hResult = m_pDevice->CreateDepthStencilState(&desc, m_arrDSS[(UINT)DSS_TYPE::LESS_EQUAL].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	{ // GRATER
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_GREATER;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		hResult = m_pDevice->CreateDepthStencilState(&desc, m_arrDSS[(UINT)DSS_TYPE::GRATER].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	{ // GRATER_EQUAL
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		hResult = m_pDevice->CreateDepthStencilState(&desc, m_arrDSS[(UINT)DSS_TYPE::GRATER_EQUAL].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	{ // NO_TEST
		desc.DepthEnable = false;
		desc.DepthFunc = D3D11_COMPARISON_NEVER;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		hResult = m_pDevice->CreateDepthStencilState(&desc, m_arrDSS[(UINT)DSS_TYPE::NO_TEST].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	{ // NO_WRITE
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		hResult = m_pDevice->CreateDepthStencilState(&desc, m_arrDSS[(UINT)DSS_TYPE::NO_WRITE].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	{ // NO_TEST_NO_WRITE
		desc.DepthEnable = false;
		desc.DepthFunc = D3D11_COMPARISON_NEVER;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		hResult = m_pDevice->CreateDepthStencilState(&desc, m_arrDSS[(UINT)DSS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	return S_OK;
}

int Device::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr; // 기본 설정과 같기 떄문에 nullptr(만들지 않아도)로 해도 된다.

	D3D11_BLEND_DESC desc = {};
	HRESULT hResult = S_OK;

	{ // ALPHABLEND
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = true;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hResult = m_pDevice->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());
		if (FAILED(hResult))
			return E_FAIL;
	}

	return S_OK;
}
