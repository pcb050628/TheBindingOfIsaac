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
	, m_fDuration(0.f)
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
	static ConstantBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	if (nullptr == m_Atlas)
	{
		g_AnimData.UseAnim2D = 0;
	}
	else
	{
		Vec2 leftop = m_Frames[m_CurFrameIdx].vLeftTop;
		Vec2 slice = m_Frames[m_CurFrameIdx].vSliceSize;
		Vec2 offset = m_Frames[m_CurFrameIdx].vOffset;
		Vec2 backround = m_Frames[m_CurFrameIdx].vBackground;

		float Atlas_X = m_Atlas->GetWidth();
		float Atlas_Y = m_Atlas->GetHeight();

		Vec2 UVLeftTop = Vec2(leftop.x / (float)Atlas_X, leftop.y / (float)Atlas_Y);;
		Vec2 UVSlice = Vec2(slice.x / (float)Atlas_X, slice.y / (float)Atlas_Y);
		Vec2 UVOffset = Vec2(offset.x / (float)Atlas_X, offset.y / (float)Atlas_Y);
		Vec2 UVBackground = Vec2(backround.x / (float)Atlas_X, backround.y / (float)Atlas_Y);

		g_AnimData.vLeftTop = UVLeftTop;
		g_AnimData.vSliceSize = UVSlice;
		g_AnimData.vOffset = UVOffset;
		g_AnimData.vBackGround = UVBackground;
		g_AnimData.UseAnim2D = 1;
	}

	pCB->SetData(&g_AnimData);
	pCB->UpdateData();

	// 10 register : anim_tex

	if (nullptr == m_Atlas)
	{
		m_Atlas->Clear(10);
	}
	else
	{
		m_Atlas->UpdateData(10);
	}
}

void Anim::LateUpdate()
{
	if (m_bIsPlaying)
	{
		m_fAccTime += Time::GetInst()->GetDeltaTime();
		
		if (m_fAccTime > m_fDuration)
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
}

void Anim::Create(Texture* _atlas, Vec2 _leftTop, Vec2 _sliceSize, Vec2 _offset, Vec2 _background, int _frmCount, int _FPS)
{
	m_Atlas = _atlas;

	for (int i = 0; i < _frmCount; ++i)
	{
		Frame frm = {};

		frm.vLeftTop = Vec2(_leftTop.x + _sliceSize.x * i, _leftTop.y); 
		frm.vSliceSize = _sliceSize; 
		frm.vOffset = _offset; 
		frm.vBackground = _background; 

		m_Frames.push_back(frm);
	}

	m_fDuration = _FPS;

	ResourceManager::GetInst()->AddResource(m_ResourceName, this);
	Save();
}

void Anim::CreateNewFrame()
{
	Frame frm = {};

	if (m_Frames.size() > 0)
	{
		int idx = m_Frames.size() - 1;

		frm.vLeftTop = m_Frames[idx].vLeftTop;
		frm.vSliceSize = m_Frames[idx].vSliceSize;
		frm.vOffset = m_Frames[idx].vOffset;
		frm.vBackground = m_Frames[idx].vBackground;
	}
	else
	{
		frm.vLeftTop = {};
		frm.vSliceSize = {};
		frm.vOffset = {};
		frm.vBackground = {};
	}

	m_Frames.push_back(frm);
	m_CurFrameIdx = m_Frames.size() - 1;
}

void Anim::RemoveCurFrame()
{
	if (m_Frames.size() < 2)
		return;

	auto iter = m_Frames.begin() + m_CurFrameIdx;
	m_Frames.erase(iter);
	m_CurFrameIdx = 0;
}

bool Anim::Load(const std::wstring& _FileName)
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(m_Type) + _FileName;
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(_FileName.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	m_ResourceName = szName;
	m_ResourcePath = _FileName;

	if (fileStream.is_open())
	{
		std::wstring line;
		int count = 0;
		int frmCount = -1;
		Frame frm = {};
		while (true)
		{
			std::getline(fileStream, line);

			if (line == L"END")
				break;

			if (frmCount != -1 && count >= frmCount)
			{
				MessageBoxW(nullptr, L"Anim ·Îµå Áß ¹º°¡ Àß¸øµÊ", L"Anim Load Failed", MB_OK);
				return false;
			}

			if (line == L"[ATLAS_PATH]")
			{
				std::getline(fileStream, line);
				m_Atlas = ResourceManager::GetInst()->Load<Texture>(line);
			}
			else if (line == L"[DURATION]")
			{
				std::getline(fileStream, line);
				m_fDuration = stoi(line);
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

		fileStream << L"[DURATION]\n" << m_fDuration << std::endl;

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
		}

		fileStream << L"END";

		fileStream.close();

		return true;
	}
	else
		return false;

	return false;
}
