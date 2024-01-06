#include "pch.h"
#include "Anim.h"

#include "Device.h"
#include "ConstantBuffer.h"

#include "Time.h"
#include "ResourceManager.h"

#include "Texture.h"

Anim::Anim() : Resource(RESOURCE_TYPE::ANIM)
	, m_Atlas(nullptr)
	, m_Frames{}
	, m_CurFrameIdx(0)
	, m_fAccTime(0.f)
	, m_bIsPlaying(false)
	, m_bIsRepeat(false)
{
}

Anim::~Anim()
{
}

void Anim::UpdateData()
{
	ConstantBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	g_AnimData.vLeftTop = m_Frames[m_CurFrameIdx].vLeftTop;
	g_AnimData.vSliceSize = m_Frames[m_CurFrameIdx].vSliceSize;
	g_AnimData.vOffset = m_Frames[m_CurFrameIdx].vOffset;
	g_AnimData.vBackGround = m_Frames[m_CurFrameIdx].vBackground;
	g_AnimData.UseAnim2D = 1;
	pCB->SetData(&g_AnimData);
	pCB->UpdateData();

	// 10 register : anim_tex
	m_Atlas->UpdateData(10);
}

void Anim::LateUpdate()
{
	m_fAccTime += Time::GetInst()->GetDeltaTime();
	
	if (m_fAccTime > m_Frames[m_CurFrameIdx].fDuration)
	{
		m_CurFrameIdx++;
		m_fAccTime = 0;
		if (m_Frames.size() <= m_CurFrameIdx)
		{
			if (m_bIsRepeat)
				m_CurFrameIdx = 0;
			else
				--m_CurFrameIdx;
		}
	}
}

void Anim::Create(Texture* _atlas, Vec2 _leftTop, Vec2 _sliceSize, Vec2 _offset, Vec2 _background, int _frmCount, int _FPS)
{
	m_Atlas = _atlas;

	for (int i = 0; i < _frmCount; ++i)
	{
		Frame frm = {};

		frm.vSliceSize = Vec2(_sliceSize.x / (float)_atlas->GetWidth(), _sliceSize.y / (float)_atlas->GetHeight());

		frm.vLeftTop = Vec2(_leftTop.x / (float)_atlas->GetWidth() + frm.vSliceSize.x * i, _leftTop.y / (float)_atlas->GetHeight());

		frm.vOffset = Vec2(_offset.x / (float)_atlas->GetWidth(), _offset.y / (float)_atlas->GetHeight());
		frm.fDuration = 1.f / _FPS;

		frm.vBackground = Vec2(_background.x / (float)_atlas->GetWidth(), _background.y / (float)_atlas->GetHeight());


		m_Frames.push_back(frm);
	}

	ResourceManager::GetInst()->AddResource(m_ResourceName, this);
}

bool Anim::Load(const std::wstring& _relativePath)
{
	filesystem::path filePath = GetContentPath() + _relativePath;
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(_relativePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	m_ResourceName = szName;
	m_ResourcePath = _relativePath;

	if (fileStream.is_open())
	{
		std::wstring line;
		int count = 0;
		int frmCount = 0;
		Frame frm = {};
		while (line != L"END")
		{
			std::getline(fileStream, line);

			if (count >= frmCount)
			{
				MessageBoxW(nullptr, L"Anim ·Îµå Áß ¹º°¡ Àß¸øµÊ", L"Anim Load Failed", MB_OK);
				return false;
			}

			if (line == L"[ATLAS_PATH]")
			{
				std::getline(fileStream, line);
				m_Atlas = ResourceManager::GetInst()->Load<Texture>(line);
			}
			else if (line == L"[FRAME_COUNT]")
			{
				std::getline(fileStream, line);
				frmCount = std::stoi(line);
			}
			else if (line == L"[LEFT_TOP_X]")
			{
				std::getline(fileStream, line);
				frm.vLeftTop.x = std::stoi(line);
			}
			else if (line == L"[LEFT_TOP_Y]")
			{
				std::getline(fileStream, line);
				frm.vLeftTop.y = std::stoi(line);
			}
			else if (line == L"[SLICE_X]")
			{
				std::getline(fileStream, line);
				frm.vSliceSize.y = std::stoi(line);
			}
			else if (line == L"[SLICE_Y]")
			{
				std::getline(fileStream, line);
				frm.vSliceSize.y = std::stoi(line);
			}
			else if (line == L"[OFFSET_X]")
			{
				std::getline(fileStream, line);
				frm.vOffset.y = std::stoi(line);
			}
			else if (line == L"[OFFSET_Y]")
			{
				std::getline(fileStream, line);
				frm.vOffset.y = std::stoi(line);
			}
			else if (line == L"[BACKGROUND_X]")
			{
				std::getline(fileStream, line);
				frm.vBackground.y = std::stoi(line);
			}
			else if (line == L"[BACKGROUND_Y]")
			{
				std::getline(fileStream, line);
				frm.vBackground.y = std::stoi(line);
			}
			else if (line == L"[DURATION]")
			{
				std::getline(fileStream, line);
				frm.fDuration = std::stof(line);

				m_Frames.push_back(frm);
				count++;
			}
		}

		fileStream.close();
		return true;
	}
	else
		return false;
}

bool Anim::Save()
{
	filesystem::path filePath = GetContentPath() + L"Resource\\Anim\\" + m_ResourceName;
	filePath += L".txt";
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		assert(m_Atlas);

		fileStream << L"[ATLAS_PATH]\n" << m_Atlas->GetResourcePath() << std::endl;

		fileStream << L"[FRAME_COUNT]\n" << m_Frames.size() << std::endl;

		for (int i = 0; i < m_Frames.size(); i++)
		{
			fileStream << L"[LEFT_TOP_X]\n" << m_Frames[i].vLeftTop.x << std::endl;
			fileStream << L"[LEFT_TOP_Y]\n" << m_Frames[i].vLeftTop.y << std::endl;

			fileStream << L"[SLICE_X]\n" << m_Frames[i].vSliceSize.x << std::endl;
			fileStream << L"[SLICE_Y]\n" << m_Frames[i].vSliceSize.y << std::endl;

			fileStream << L"[OFFSET_X]\n" << m_Frames[i].vOffset.x << std::endl;
			fileStream << L"[OFFSET_Y]\n" << m_Frames[i].vOffset.y << std::endl;

			fileStream << L"[BACKGROUND_X]\n" << m_Frames[i].vBackground.x << std::endl;
			fileStream << L"[BACKGROUND_Y]\n" << m_Frames[i].vBackground.y << std::endl;

			fileStream << L"[DURATION]\n" << m_Frames[i].fDuration << std::endl;
		}

		fileStream << L"END";

		fileStream.close();

		return true;
	}
	else
		return false;

	return false;
}
