#include "pch.h"
#include "AssetManager.h"
#include <filesystem>

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::Init()
{
    std::wstring directoryPath = L"..\\Assets";
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
    {
        m_AssetPaths.insert(std::make_pair((AssetID)std::stoi(entry.path().filename()), entry.path()));
    }
}
