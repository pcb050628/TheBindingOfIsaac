#include "pch.h"
#include "Asset.h"
#include "AssetManager.h"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::Init()
{
    std::wstring directoryPath = GetContentPath();
    directoryPath += L"Asset\\";

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
    {
        //if (entry.is_directory())
        //{
        //}

        //for (const auto& files : std::filesystem::directory_iterator(entry.path()))
        //{
        //    m_AssetPaths.insert(std::make_pair((AssetID)std::stoi(files.path().filename()), files.path()));
        //}

        m_AssetPaths.insert(std::make_pair((AssetID)std::stoi(entry.path().filename()), entry.path()));
    }
}

std::wstring GetContentPath()
{
    wchar_t path[500];
    GetModuleFileNameW(nullptr, path, 500);
    std::wstring directoryPath(path);

    for (int i = 0; i < 2; i++)
    {
        size_t point = directoryPath.find_last_of(L"\\");

        directoryPath = directoryPath.substr(0, point);
    }

    return directoryPath + L"\\Content\\";
}