#pragma once

class GUI;
class ImGuiManager
{
	SINGLETON(ImGuiManager)
private:
	std::map<std::string, GUI*> m_mapGUI;

public:
	void Init(HWND _hMainWnd, Microsoft::WRL::ComPtr<ID3D11Device> _Device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context);
	void Progress();

private:
	void Update();
	void Render();

};

