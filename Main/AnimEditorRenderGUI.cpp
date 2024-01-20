#include "pch.h"
#include "AnimEditorRenderGUI.h"

#include "AnimEditorGUI.h"

#include <Engine/Device.h>
#include <Engine/ResourceManager.h>
#include <Engine/Anim.h>
#include <Engine/Texture.h>

AnimEditorRenderGUI::AnimEditorRenderGUI() : GUI("AnimEditorRender", "##AnimEditorRenderGUI")
	, m_Target(nullptr)
{
	Deactivate();
}

AnimEditorRenderGUI::~AnimEditorRenderGUI()
{
}

void AnimEditorRenderGUI::RenderUpdate()
{
	if (m_Target && m_Target->m_EditAnim->GetAtlas())
	{
		Anim* anim = m_Target->m_EditAnim;
		Texture* image = anim->GetAtlas();

		ImGui::Text("Atlas"); ImGui::Spacing();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
		ImGuiIO& io = ImGui::GetIO();

		//현재 아틀라스 그리기 / Rect 같이 그려서 안에 넣기 ( 아직 안함 )
		ImVec2 maxImageSize = ImGui::GetWindowSize();
		maxImageSize.y /= 2;
		ImVec2 windowPos = ImGui::GetWindowPos() + ImVec2(0, 50);
		ImVec2 LeftTop = ImVec2(100, 0);
		ImVec2 transformedImageSize = ImVec2(image->GetWidth(), image->GetHeight());

		SizeCheck(transformedImageSize, maxImageSize);
		draw_list->AddImage(image->GetSRV().Get(), LeftTop+windowPos, LeftTop + transformedImageSize + windowPos);

		ImVec2 imageDelta = ImVec2(transformedImageSize.x / image->GetWidth(), transformedImageSize.y / image->GetHeight());

		//마우스 클릭으로 이미지 프레임 만들기
		if (ImGui::InvisibleButton("canvas", LeftTop + transformedImageSize))
		{
			//마우스 위치 얻기
			ImVec2 mouse_pos_in_canvas(io.MousePos.x, io.MousePos.y);
			if (m_bMouseClick)
			{
				m_MouseClickPixel = mouse_pos_in_canvas - LeftTop;
				ImGui::GetMouseCursor();
			}
			mouse_pos_in_canvas -= canvas_p0;
			mouse_pos_in_canvas -= LeftTop;

			//mosuePos -> PixelPos
			//imageDelta 로 비율 조정하고 transformedImageSize로 UV값 얻고 UV값으로 픽셀값 얻기
			ImVec2 UV = ImVec2(mouse_pos_in_canvas.x / transformedImageSize.x, mouse_pos_in_canvas.y / transformedImageSize.y); //ImVec2(mousePosInImage.x / transformedImageSize.x, mousePosInImage.y / transformedImageSize.y);
			Vec2 Pixel = Vec2(UV.x * image->GetWidth(), UV.y * image->GetHeight());
			Pixel.x = (int)Pixel.x;
			Pixel.y = (int)Pixel.y;
			Pixel.x += 10;

			// 현재 프레임 정보
			Frame nFrame = anim->GetCurFrame();

			//pixel 탐색에서 사용
			FrameRect rect = { Pixel, Pixel };
			std::set<Vec2> check;

			//Image 복사 후 매핑
			D3D11_TEXTURE2D_DESC desc = image->GetDesc();
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.BindFlags = 0;

			Texture* pTex = new Texture;
			pTex->Create(desc.Width, desc.Height, desc.Format, desc.BindFlags, desc.Usage);

			D3D11_MAPPED_SUBRESOURCE data = {};
			Device::GetInst()->GetContext()->CopyResource(pTex->GetTex2D().Get(), image->GetTex2D().Get());
			Device::GetInst()->GetContext()->Map(pTex->GetTex2D().Get(), 0, D3D11_MAP_READ, 0, &data);

			MakeFrameByPixelCoord(Pixel, rect, data, check);

			Device::GetInst()->GetContext()->Unmap(pTex->GetTex2D().Get(), 0);
			delete pTex;

			//현재 프레임이 잘리지 않은 경우
			if (0 == nFrame.vSliceSize.x || 0 == nFrame.vSliceSize.y)
			{
				//현재 프레임을 설졍함
				Frame& CurFrame = anim->GetCurFrame();
				
				CurFrame.vLeftTop = rect.LT;
				CurFrame.vSliceSize = rect.RB - rect.LT;
				CurFrame.vBackground = nFrame.vSliceSize;
			}
			//현재 프레임이 이미 잘려 있는 경우
			else
			{
				// 새로운 프레임을 만들어서 추가함
				Frame frm = {};

				frm.vLeftTop = rect.LT;
				frm.vSliceSize = rect.RB - rect.LT;
				frm.vBackground = nFrame.vSliceSize;

				anim->AddFrame(frm);
			}
		}

		ImVec2 rect = ImVec2(10, 10);
		ImColor col = ImVec4(1.f, 1.f, 0.f, 1.f);
		draw_list->AddRect(m_MouseClickPixel - rect, m_MouseClickPixel + rect, col);

		std::vector<Frame> frames = anim->GetAllFrame();
		std::vector<Frame>::iterator iter = frames.begin();

		//모든 프레임 가져와서 초록색으로 선 그어주기 / Rect 같이 그려서 안에 넣기 ( 아직 안함 )
		for (; iter != frames.end(); iter++)
		{
			ImVec2 leftTop		= ImVec2(iter->vLeftTop.x	, iter->vLeftTop.y)		* imageDelta;
			ImVec2 sliceSize	= ImVec2(iter->vSliceSize.x	, iter->vSliceSize.y)	* imageDelta;
			ImVec2 background	= ImVec2(iter->vBackground.x, iter->vBackground.y)	* imageDelta;
			ImVec2 offset		= ImVec2(iter->vOffset.x	, iter->vOffset.y)		* imageDelta;

			ImColor col = ImVec4(0.f, 1.f, 0.f, 1.f);
			draw_list->AddRect(LeftTop + windowPos + leftTop , LeftTop + windowPos + leftTop + sliceSize , col);

			col = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec2 BackgroundLeftTop = leftTop + (sliceSize / 2) - (background / 2);
			draw_list->AddRect(LeftTop + windowPos + BackgroundLeftTop - offset, LeftTop + windowPos + BackgroundLeftTop + background - offset, col);
		}

		//현재 프레임 띄우기
		{
			//frame
			Frame curFrame = anim->GetCurFrame();

			ImVec2 leftTop		= ImVec2(curFrame.vLeftTop.x, curFrame.vLeftTop.y);
			ImVec2 sliceSize	= ImVec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);
			ImVec2 offset		= ImVec2(curFrame.vOffset.x, curFrame.vOffset.y);
			ImVec2 background	= ImVec2(curFrame.vBackground.x, curFrame.vBackground.y);

			//UV
			ImVec2 ImageSize	= ImVec2(image->GetWidth(), image->GetHeight());
			ImVec2 leftTopUV	= ImVec2(leftTop.x / ImageSize.x	, leftTop.y / ImageSize.y);	
			ImVec2 sliceSizeUV	= ImVec2(sliceSize.x / ImageSize.x	, sliceSize.y / ImageSize.y);
			ImVec2 offsetUV		= ImVec2(offset.x / ImageSize.x		, offset.y / ImageSize.y);
			ImVec2 backgroundUV = ImVec2(background.x / ImageSize.x	, background.y / ImageSize.y);

			//background in render uv
			ImVec2 BackgroundLeftTopUV = leftTopUV + (sliceSizeUV / 2) - (backgroundUV / 2) - offsetUV;
			ImVec2 BackgroundRightBotUV = BackgroundLeftTopUV + backgroundUV;

			//drawPos
			ImVec2 drawLeftTop = LeftTop + windowPos + maxImageSize + ImVec2(-250, 0);
			ImVec2 drawRightBot = drawLeftTop + ImVec2(100, 100);

			//backgroundUV - LeftTopUV = Delta
			ImVec2 DeltaLeftTopUV = leftTopUV - BackgroundLeftTopUV;
			ImVec2 DeltaLeftTop = ImVec2(DeltaLeftTopUV.x * ImageSize.x, DeltaLeftTopUV.y * ImageSize.y);

			ImVec2 DeltaRightBotUV = BackgroundRightBotUV - (leftTopUV + sliceSizeUV);
			ImVec2 DeltaRightBot = ImVec2(DeltaRightBotUV.x * ImageSize.x, DeltaRightBotUV.y * ImageSize.y);

			//final drawPos
			drawLeftTop += DeltaLeftTop;
			drawRightBot -= DeltaRightBot;

			ImVec2 rightBotUV = leftTopUV + sliceSizeUV;

			if (drawLeftTop.x > drawRightBot.x)
			{
				float sizeX = drawLeftTop.x - drawRightBot.x;
				float tmpX = drawLeftTop.x;
				drawLeftTop.x = drawRightBot.x;
				drawRightBot.x = tmpX;

				drawLeftTop.x += (sizeX / 2);
				drawRightBot.x -= (sizeX / 2);
			}

			if (drawLeftTop.y > drawRightBot.y)
			{
				float sizeY = drawLeftTop.y - drawRightBot.y;
				float tmpY = drawLeftTop.y;
				drawLeftTop.y = drawRightBot.y;
				drawRightBot.y = tmpY;

				drawLeftTop.y += (sizeY / 2);
				drawRightBot.y -= (sizeY / 2);
			}

			draw_list->AddImage(image->GetSRV().Get(), drawLeftTop, drawRightBot, leftTopUV, rightBotUV);
		}
	}
}

void AnimEditorRenderGUI::SizeCheck(ImVec2& _size, ImVec2 _maxSize)
{
	if ((int)_size.x > (int)_maxSize.x)
	{
		float eWidth = _size.x;
		float eHeight = _size.y;

		float nHeight = _maxSize.y;

		_size.x = _maxSize.x;
		_size.y = _size.y * (_size.x / eWidth);

		SizeCheck(_size, _maxSize);
	}
	else if ((int)_size.y > (int)_maxSize.y)
	{
		float eWidth = _size.x;
		float eHeight = _size.y;

		float nWidth = _maxSize.x;

		_size.y = _maxSize.y;
		_size.x = _size.x * (_size.y / eWidth);

		SizeCheck(_size, _maxSize);
	}
}

void AnimEditorRenderGUI::MakeFrameByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check)
{
	int x = (int)_pixelPos.x; // Pixel x-coordinate
	int y = (int)_pixelPos.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch;
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);

	// Access RGBA values
	BYTE alpha = pData[3];

	if (!alpha)
	{
		for (int i = 0; i < 30; i++)
		{
			x += 1;

			UINT rowPitch = _mappedSub.RowPitch;
			BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);

			// Access RGBA values
			alpha = pData[3];
			if (alpha)
			{
				break;
			}
		}

		return;
	}

	GetLeftByPixelCoord(_pixelPos, _frameOut, _mappedSub, _check);
	_check.clear();
	GetRightByPixelCoord(_pixelPos, _frameOut, _mappedSub, _check);
	_check.clear();
	GetTopByPixelCoord(_pixelPos, _frameOut, _mappedSub, _check);
	_check.clear();
	GetBottomByPixelCoord(_pixelPos, _frameOut, _mappedSub, _check);
	_check.clear();
}

void AnimEditorRenderGUI::GetLeftByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check)
{
	_check.insert(_pixelPos);
	int x = (int)_pixelPos.x; // Pixel x-coordinate
	int y = (int)_pixelPos.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch;
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);

	// Access RGBA values
	BYTE alpha = pData[3];

	//left
	if (x < _frameOut.LT.x)
	{
		_frameOut.LT.x = x;
	}

	if (!alpha)
		return;

	static Vec2 dir[3] = { Vec2(-1, 0), Vec2(0, -1), Vec2(0, 1) };
	for (int i = 0; i < 3; i++)
	{
		Vec2 coord = _pixelPos + dir[i];

		if (_check.find(coord) == _check.end())
		{
			GetLeftByPixelCoord(coord, _frameOut, _mappedSub, _check);
		}
	}
}

void AnimEditorRenderGUI::GetRightByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check)
{
	_check.insert(_pixelPos);
	int x = (int)_pixelPos.x; // Pixel x-coordinate
	int y = (int)_pixelPos.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch;
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);

	// Access RGBA values
	BYTE alpha = pData[3];

	//right
	if (x > _frameOut.RB.x)
	{
		_frameOut.RB.x = x;
	}

	if (!alpha)
		return;

	static Vec2 dir[3] = { Vec2(1, 0), Vec2(0, 1), Vec2(0, -1) };
	for (int i = 0; i < 3; i++)
	{
		Vec2 coord = _pixelPos + dir[i];

		if (_check.find(coord) == _check.end())
		{
			GetRightByPixelCoord(coord, _frameOut, _mappedSub, _check);
		}
	}
}

void AnimEditorRenderGUI::GetTopByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check)
{
	_check.insert(_pixelPos);
	int x = (int)_pixelPos.x; // Pixel x-coordinate
	int y = (int)_pixelPos.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch;
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);

	// Access RGBA values
	BYTE alpha = pData[3];

	//top
	if (y < _frameOut.LT.y)
	{
		_frameOut.LT.y = y;
	}

	if (!alpha)
		return;

	static Vec2 dir[3] = { Vec2(0, -1), Vec2(-1, 0), Vec2(1, 0) };
	for (int i = 0; i < 3; i++)
	{
		Vec2 coord = _pixelPos + dir[i];

		if (_check.find(coord) == _check.end())
		{
			GetTopByPixelCoord(coord, _frameOut, _mappedSub, _check);
		}
	}
}

void AnimEditorRenderGUI::GetBottomByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check)
{
	_check.insert(_pixelPos);
	int x = (int)_pixelPos.x; // Pixel x-coordinate
	int y = (int)_pixelPos.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch;
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);

	// Access RGBA values
	BYTE alpha = pData[3];

	//bottom
	if (y > _frameOut.RB.y)
	{
		_frameOut.RB.y = y;
	}

	if (!alpha)
		return;

	static Vec2 dir[3] = { Vec2(0, 1), Vec2(-1, 0), Vec2(1, 0) };
	for (int i = 0; i < 3; i++)
	{
		Vec2 coord = _pixelPos + dir[i];

		if (_check.find(coord) == _check.end())
		{
			GetBottomByPixelCoord(coord, _frameOut, _mappedSub, _check);
		}
	}
}
