#pragma once
#include "pch.h"
#include "define.h"
#include "wrl.h"
#include "d3d11_1.h"
#include "directxtk/SpriteBatch.h"

class RenderManager
{
	SINGLETON(RenderManager)
private:
	Microsoft::WRL::ComPtr<ID3D11Device1> mp_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> mp_Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> mp_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mp_RenderTargetView;
	std::unique_ptr<DirectX::SpriteBatch> mp_SpriteBatch;
	

public:
	void Init(HWND _hwnd);
	void Update();
	void Render();

};

