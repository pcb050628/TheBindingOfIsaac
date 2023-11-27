#pragma once
#include "pch.h"
#include "define.h"
#include "wrl.h"
#include <d3d11_1.h>
#include "directxtk/SpriteBatch.h"
#include "directxtk/SpriteFont.h"

class RenderManager
{
	SINGLETON(RenderManager)
private:
	Microsoft::WRL::ComPtr<ID3D11Device1> mp_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> mp_Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> mp_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mp_RenderTargetView;
	std::unique_ptr<DirectX::SpriteBatch> mp_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mp_SpriteFont;

	bool isDrawing;
	
public:
	Microsoft::WRL::ComPtr<ID3D11Device1> GetDevice() { return mp_Device; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> GetContext() { return mp_Context; }
	bool GetIsDrawing() { return isDrawing; }

public:
	void Init(HWND _hwnd);
	void Update();
	void TextureRender(ID3D11ShaderResourceView* _srv, struct Vec2 _pos, RECT& _iSection);
	void FontRender(std::wstring _wstring, Vec2 _pos, DirectX::XMVECTORF32 _color = DirectX::Colors::White);

	void PrepareDraw();
	void StartDraw() { isDrawing = true; mp_SpriteBatch->Begin(); }
	void EndDraw() { isDrawing = false; mp_SpriteBatch->End(); }
};

