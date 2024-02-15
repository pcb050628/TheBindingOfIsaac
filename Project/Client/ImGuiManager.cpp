#include "pch.h"
#include "ImGuiManager.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <Engine/ChapterManager.h>

#include "GUI.h"

#include "DockSpaceGUI.h"
#include "InspectorGUI.h"
#include "OutlinerGUI.h"
#include "ListGUI.h"
#include "EditorGUI.h"
#include "AnimEditorRenderGUI.h"
#include "RoomEditorGUI.h"
#include "RoomEditorRenderGUI.h"

ImGuiManager::ImGuiManager()
{}
ImGuiManager::~ImGuiManager()
{
    for (auto pair : m_mapGUI)
    {
        if (nullptr != pair.second)
        {
            delete pair.second;
        }
    }
    m_mapGUI.clear();
}

void ImGuiManager::Init(HWND _hMainWnd, Microsoft::WRL::ComPtr<ID3D11Device> _Device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context)
{
    // ImGui �ʱ�ȭ
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hMainWnd);
    ImGui_ImplDX11_Init(_Device.Get(), _Context.Get());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    CrateGUI();
}

void ImGuiManager::Progress()
{
	Update();
	Render();
}

void ImGuiManager::Update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    std::map<std::string, GUI*>::iterator iter = m_mapGUI.begin();
    for (; iter != m_mapGUI.end(); iter++)
    {
        iter->second->Update();
    }
}

void ImGuiManager::Render()
{
    std::map<std::string, GUI*>::iterator iter = m_mapGUI.begin();
    for (; iter != m_mapGUI.end(); iter++)
    {
        iter->second->Render();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImGuiManager::CrateGUI()
{
    GUI* gui = new InspectorGUI;
    m_mapGUI.insert(std::make_pair(gui->GetID(), gui));

    gui = new OutlinerGUI;
    m_mapGUI.insert(std::make_pair(gui->GetID(), gui));

    gui = new ListGUI;
    m_mapGUI.insert(std::make_pair(gui->GetID(), gui));

    gui = new RoomEditorGUI;
    m_mapGUI.insert(std::make_pair(gui->GetID(), gui));

    //gui = new EditorGUI;
    //m_mapGUI.insert(std::make_pair(gui->GetID(), gui));
    //
    //gui = new AnimEditorRenderGUI;
    //m_mapGUI.insert(std::make_pair(gui->GetID(), gui));
}
