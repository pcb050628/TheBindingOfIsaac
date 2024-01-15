#pragma once

class ImGuiManager
{
	SINGLETON(ImGuiManager)
private:

public:
	void Init(HWND _hMainWnd, Microsoft::WRL::ComPtr<ID3D11Device> _Device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context);
	void Progress();

private:
	void Update();
	void Render();

};

