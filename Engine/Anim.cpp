#include "pch.h"
#include "Anim.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "Time.h"
#include "Texture.h"


Anim::Anim() : Super(AssetType::ANIM)
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

	while (true)
	{
		wchar_t szRead[256] = {};
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
		{
			break;
		}

		if (!wcscmp(szRead, L"[ANIM_NAME]"))
		{
			std::wstring name;

			fwscanf_s(pFile, L"%s", szRead, 256);

			name = szRead;
			Super::SetAssetName(name);
		}
		else if (!wcscmp(szRead, L"[ANIM_PATH]"))
		{
			std::wstring path;

			fwscanf_s(pFile, L"%s", szRead, 256);

			path = szRead; 
			Super::SetAssetPath(path);
		}
		else if (!wcscmp(szRead, L"[ATLAS_TEXTURE_NAME]"))
		{
			std::wstring name;

			fwscanf_s(pFile, L"%s", szRead, 256);

			name = szRead;
			Super::SetAssetName(name);
		}
		else if (!wcscmp(szRead, L"[ATLAS_TEXTURE_PATH]"))
		{
			std::wstring strPath;

			fwscanf_s(pFile, L"%s", szRead, 256);

			if (m_Atlas == nullptr)
			{
				strPath = szRead;
				m_Atlas = ResourceManager::GetInst()->LoadByPath<ShaderTextureResource>(Super::GetAssetName(), strPath);
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

bool Anim::Save()
{
	FILE* pFile = nullptr;

	std::wstring _FilePath = Super::GetAssetPath() + std::to_wstring((UINT)Super::GetAssetID());

	_wfopen_s(&pFile, _FilePath.c_str(), L"w");

	/*if (nullptr == pFile)
	{
		LOG(ERR, L"파일 열기 실패");
		return false;
	}*/

	// 경로
	fwprintf_s(pFile, L"[ANIM_NAME]\n");

	fwprintf_s(pFile, Super::GetAssetName().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ANIM_PATH]\n");

	fwprintf_s(pFile, Super::GetAssetPath().c_str());
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
	if (m_bFinish || !mb_isPlaying)
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
	if (m_bFinish || !mb_isPlaying)
		return;
	Vec2 Offset = m_Frames[m_iCurFrame].vOffset;
	Vec2 LeftTop = m_Frames[m_iCurFrame].vLeftTop;
	Vec2 RightBottom = m_Frames[m_iCurFrame].vLeftTop + m_Frames[m_iCurFrame].vCutSize;

	RECT iSection = { LeftTop.x, LeftTop.y, RightBottom.x, RightBottom.y };

	RenderManager::GetInst()->TextureRender(m_Atlas->GetTextureView().Get(), _pos + Offset, iSection);
}
 