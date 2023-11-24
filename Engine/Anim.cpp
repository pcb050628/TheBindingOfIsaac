#include "pch.h"
#include "Anim.h"
#include "ResourceManager.h"
#include "Time.h"
#include "Texture.h"


Anim::Anim() : Super(ResourceType::Anim)
    , m_Frames({})
    , m_Atlas(nullptr)
{
}

Anim::~Anim()
{
}

bool Anim::Load(std::wstring _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"r");

	if (nullptr == pFile)
	{
		//LOG(ERR, L"파일 열기 실패");
		return false;
	}

	// Animation 이름 로드

	while (true)
	{
		wchar_t szRead[256] = {};
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
		{
			break;
		}

		if (!wcscmp(szRead, L"[ANIM_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			Super::SetResourceName(szRead);
		}
		else if (!wcscmp(szRead, L"[ATLAS_TEXTURE_NAME]"))
		{
			std::wstring strName;

			fwscanf_s(pFile, L"%s", szRead, 256);
			strName = szRead;

			m_Atlas = ResourceManager::GetInst()->Load<Texture>(strName);
		}
		else if (!wcscmp(szRead, L"[ATLAS_TEXTURE_PATH]"))
		{
			std::wstring strPath;

			fwscanf_s(pFile, L"%s", szRead, 256);

			if (m_Atlas == nullptr)
			{
				strPath = szRead;
				m_Atlas = ResourceManager::GetInst()->LoadByPath<Texture>(strPath);
			}
		}
		else if (!wcscmp(szRead, L"[FRAME_COUNT]"))
		{
			int iFrameCount = 0;
			fwscanf_s(pFile, L"%d", &iFrameCount);
			m_Frames.resize(iFrameCount);

			int iCurFrame = 0;
			while (true)
			{
				fwscanf_s(pFile, L"%s", szRead, 256);

				if (!wcscmp(szRead, L"[FRAME_NUM]"))
				{
					fwscanf_s(pFile, L"%d", &iCurFrame);
				}
				else if (!wcscmp(szRead, L"[LEFT_TOP]"))
				{
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].vLeftTop.x);
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].vLeftTop.y);
				}
				else if (!wcscmp(szRead, L"[CUT_SIZE]"))
				{
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].vCutSize.x);
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].vCutSize.y);
				}
				else if (!wcscmp(szRead, L"[OFFSET]"))
				{
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].vOffset.x);
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].vOffset.y);
				}
				else if (!wcscmp(szRead, L"[DURATION]"))
				{
					fwscanf_s(pFile, L"%f", &m_Frames[iCurFrame].Duration);

					if (iFrameCount - 1 <= iCurFrame)
						break;
				}
			}
		}
	}

	fclose(pFile);
	return true;
}

Anim* Anim::Create(std::wstring _ResourcePath)
{


	return this;
}

bool Anim::Save()
{
	FILE* pFile = nullptr;

	std::wstring _FilePath = Super::GetResourcePath();

	_wfopen_s(&pFile, _FilePath.c_str(), L"w");

	/*if (nullptr == pFile)
	{
		LOG(ERR, L"파일 열기 실패");
		return false;
	}*/

	// 리소스 이름 저장
	fwprintf_s(pFile, L"[ANIM_NAME]\n");

	std::wstring strName = Super::GetResourceName();
	fwprintf_s(pFile, strName.c_str());
	fwprintf_s(pFile, L"\n\n");

	// 경로
	fwprintf_s(pFile, L"[ANIM_PATH]\n");

	fwprintf_s(pFile, Super::GetResourcePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	// atlas
	fwprintf_s(pFile, L"[ATLAS_TEXTURE_NAME]\n");
	fwprintf_s(pFile, m_Atlas->GetResourceName().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_TEXTURE_PATH]\n");
	fwprintf_s(pFile, m_Atlas->GetResourcePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[FRAME_COUNT]\n");
	fwprintf_s(pFile, L"%d\n\n", (int)m_Frames.size());

	for (int i = 0; i < (int)m_Frames.size(); ++i) 
	{
		fwprintf_s(pFile, L"[FRAME_NUM]\n");
		fwprintf_s(pFile, L"%d\n", i);

		fwprintf_s(pFile, L"[LEFT_TOP]\n");
		fwprintf_s(pFile, L"%f %f\n", m_Frames[i].vLeftTop.x, m_Frames[i].vLeftTop.y);

		fwprintf_s(pFile, L"[CUT_SIZE]\n");
		fwprintf_s(pFile, L"%f %f\n", m_Frames[i].vCutSize.x, m_Frames[i].vCutSize.y);

		fwprintf_s(pFile, L"[OFFSET]\n");
		fwprintf_s(pFile, L"%f %f\n", m_Frames[i].vOffset.x, m_Frames[i].vOffset.y);

		fwprintf_s(pFile, L"[DURATION]\n");
		fwprintf_s(pFile, L"%f\n\n", m_Frames[i].Duration);
	}

	fclose(pFile);

	return true;
}

void Anim::LateUpdate()
{
	if (m_bFinish)
		return;

	m_AccTime += Time::GetInst()->GetDeltaTime();

	if (m_Frames[m_iCurFrame].Duration < m_AccTime)
	{
		m_AccTime = 0.f;

		if (m_Frames.size() - 1 <= m_iCurFrame)
		{
			m_bFinish = true;
		}
		else
		{
			++m_iCurFrame;
		}
	}
}

void Anim::Render(Vec2 _pos)
{

}
