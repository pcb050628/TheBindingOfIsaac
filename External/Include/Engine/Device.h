#pragma once

class ConstantBuffer;
class Device
{
	SINGLETON(Device)
private:
	Microsoft::WRL::ComPtr<ID3D11Device>			m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_pContext;

	HWND											m_hRenderWnd;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_pSwapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRTView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pRTTex;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDSView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDSTex;

	ConstantBuffer*									m_arrConstantBuffer[(UINT)CB_TYPE::END];

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_arrDSS[(UINT)DSS_TYPE::END];
	Microsoft::WRL::ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_arrSS[2];

	Vec2											m_vResolution;
	Vec4											m_vClearColor;
	
public:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return m_pContext; }

	ConstantBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrConstantBuffer[(UINT)_type]; }

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> GetRasterizerState(RS_TYPE _type) { return m_arrRS[(UINT)_type]; }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetDepthStencilState(DSS_TYPE _type) { return m_arrDSS[(UINT)_type]; }
	Microsoft::WRL::ComPtr<ID3D11BlendState> GetBlendState(BS_TYPE _type) { return m_arrBS[(UINT)_type]; }

public:
	int Init(HWND _hwnd, Vec2 _resolution);
	
	void DrawStart();
	void DrawEnd() { m_pSwapChain->Present(0, 0); }

	void SetClearColor(Vec4 norm_color) { m_vClearColor = norm_color; }

	Vec2 GetResolution() { return m_vResolution; }

private:
	int CreateSwapChain();
	int CreateRenderTargetView();
	int CreateDepthStencilView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
};

