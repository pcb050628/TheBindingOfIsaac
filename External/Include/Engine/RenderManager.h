#pragma once

class RenderManager
{
	SINGLETON(RenderManager)
private:
	Microsoft::WRL::ComPtr<ID3D11Device1> mp_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> mp_Context;
	//D3D11_VIEWPORT m_ViewPort; // 

	Microsoft::WRL::ComPtr<IDXGISwapChain1> mp_SwapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mp_RTView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mp_RTTex;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mp_DSView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mp_DSTex;

	std::unique_ptr<DirectX::SpriteBatch> mp_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mp_SpriteFont;

	Vec2 m_Resolution;
	Vec4 m_ClearColor;

	bool isDrawing;
	
public:
	Microsoft::WRL::ComPtr<ID3D11Device1> GetDevice() { return mp_Device; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> GetContext() { return mp_Context; }
	bool GetIsDrawing() { return isDrawing; }

public:
	void Init(HWND _hwnd, Vec2 _resolution);
	void Update();
	void TextureRender(ID3D11ShaderResourceView* _srv, Vec2 _pos, RECT& _iSection);
	void FontRender(std::wstring _wstring, Vec2 _pos, DirectX::XMVECTORF32 _color = DirectX::Colors::White);

	void PrepareDraw();
	void StartDraw() { isDrawing = true; mp_SpriteBatch->Begin(); }
	void EndDraw() { isDrawing = false; mp_SpriteBatch->End(); mp_SwapChain->Present(0, 0); }

	void SetClearColor(Vec4 norm_color) { m_ClearColor = norm_color; }
};

