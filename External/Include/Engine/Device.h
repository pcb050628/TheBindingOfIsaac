#pragma once

class ConstantBuffer;
class Device
{
	SINGLETON(Device)
private:
	Microsoft::WRL::ComPtr<ID3D11Device1>			m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>	m_pContext;
	//Microsoft::WRL::ComPtr<D3D11_VIEWPORT> m_pViewPort;
	//std::vector<Microsoft::WRL::ComPtr<D3D11_VIEWPORT>> m_pViewPort; // 뷰포트 여러개 필요할 것으로 생각됨

	Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_pSwapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_pRTView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pRTTex;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_pDSView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_pDSTex;

	ConstantBuffer*									m_ConstantBuffer[(UINT)CB_TYPE::END];

	Vec2											m_vResolution;
	Vec4											m_vClearColor;
	
public:
	Microsoft::WRL::ComPtr<ID3D11Device1> GetDevice() { return m_pDevice; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> GetContext() { return m_pContext; }

	ConstantBuffer* GetConstBuffer(CB_TYPE _type) { return m_ConstantBuffer[(UINT)_type]; }

public:
	void Init(HWND _hwnd, Vec2 _resolution);
	
	void DrawStart();
	void DrawEnd() { m_pSwapChain->Present(0, 0); }

	void SetClearColor(Vec4 norm_color) { m_vClearColor = norm_color; }

private:
	int CreateConstBuffer();
};

