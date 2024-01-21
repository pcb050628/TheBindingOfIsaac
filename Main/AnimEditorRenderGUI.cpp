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
	if (m_Target && m_Target->IsActive() && m_Target->m_EditAnim->GetAtlas())
	{
		Anim* anim = m_Target->m_EditAnim;
		Texture* image = anim->GetAtlas();

		ImGui::Text("Atlas"); ImGui::Spacing();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
		ImGuiIO& io = ImGui::GetIO();

		//���� ��Ʋ�� �׸��� / Rect ���� �׷��� �ȿ� �ֱ� ( ���� ���� )
		ImVec2 maxImageSize = ImGui::GetWindowSize();
		maxImageSize.y /= 2;
		ImVec2 windowPos = ImGui::GetWindowPos() + ImVec2(0, 50);
		ImVec2 LeftTop = ImVec2(100, 0);
		ImVec2 transformedImageSize = ImVec2(image->GetWidth(), image->GetHeight());

		SizeCheck(transformedImageSize, maxImageSize);
		draw_list->AddImage(image->GetSRV().Get(), LeftTop + windowPos, LeftTop + transformedImageSize + windowPos);

		ImVec2 imageDelta = ImVec2(transformedImageSize.x / image->GetWidth(), transformedImageSize.y / image->GetHeight());

		//���콺 Ŭ������ �̹��� ������ �����
		ImGui::InvisibleButton("canvas", LeftTop + transformedImageSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

		//��Ŭ��
		if (ImGui::IsKeyDown(ImGuiKey_MouseRight))
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				//���콺 ��ġ ���
				//���콺 ��Ŭ���� ó�� �Ͼ�� ��
				if (!m_bMouseRightClick)
				{
					Vec2 imageSize(image->GetWidth(), image->GetHeight());
					ImVec2 mouse_pos_in_canvas(io.MousePos.x, io.MousePos.y);
					ImVec2 startPos = mouse_pos_in_canvas;
					startPos -= windowPos;
					startPos -= LeftTop;
					ImVec2 startUV = ImVec2(startPos.x / transformedImageSize.x, startPos.y / transformedImageSize.y);
					m_MouseClickPixel = Vec2(startUV.x * imageSize.x, startUV.y * imageSize.y);
					m_MouseClickPixel = Vec2((int)m_MouseClickPixel.x, (int)m_MouseClickPixel.y);
				}
				m_bMouseRightClick = true;
			}
		}
		else
		{
			m_bMouseRightClick = false;
		}


		//��Ŭ��
		if (ImGui::IsKeyDown(ImGuiKey_MouseLeft))
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				m_bMouseLeftClick = true;
				//���콺 ��ġ ���
				ImVec2 mouse_pos_in_canvas(io.MousePos.x, io.MousePos.y);
				mouse_pos_in_canvas -= windowPos;
				mouse_pos_in_canvas -= LeftTop;

				//mosuePos -> PixelPos
				//UV�� ��� UV������ �ȼ��� ���
				ImVec2 UV = ImVec2(mouse_pos_in_canvas.x / transformedImageSize.x, mouse_pos_in_canvas.y / transformedImageSize.y); //ImVec2(mousePosInImage.x / transformedImageSize.x, mousePosInImage.y / transformedImageSize.y);
				Vec2 Pixel = Vec2(UV.x * image->GetWidth(), UV.y * image->GetHeight());
				Pixel.x = (int)Pixel.x;
				Pixel.y = (int)Pixel.y;

				//���� �ȼ���ġ�� lefttop + sliceSize �� ��ġ�� �ش��ϴ� �������� �ִ��� �˻�
				std::vector<Frame> vecFrame = anim->GetAllFrame();
				int maxIdx = vecFrame.size();
				bool IsExist = false;
				int idx = 0;
				for (; idx < maxIdx; idx++)
				{
					Vec2 LT = vecFrame[idx].vLeftTop;
					Vec2 RB = vecFrame[idx].vLeftTop + vecFrame[idx].vSliceSize;

					if (Pixel.x > LT.x && Pixel.y > LT.y
						&& Pixel.x < RB.x && Pixel.y < RB.y)
					{
						IsExist = true;
						break;
					}
				}

				//�ȼ���ġ�� �ش��ϴ� �������� �ִٸ� �� �������� ���� ���������� �ٲ�
				if (IsExist)
				{
					anim->SetCurFrameIdx(idx);
				}
				//������ �ȼ���ġ�� ������ ���� �Ǵ� ������ ���� ����
				else
				{
					// ���� ������ ����
					Frame nFrame = anim->GetCurFrame();

					//pixel Ž������ ���
					FrameRect rect = { Pixel, Pixel };
					std::set<Vec2> check;

					//Image ���� �� ����
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

					//���� �������� �߸��� ���� ���
					if (0 == nFrame.vSliceSize.x || 0 == nFrame.vSliceSize.y)
					{
						//���� �������� ������
						Frame& CurFrame = anim->GetCurFrame();

						CurFrame.vLeftTop = rect.LT;
						CurFrame.vSliceSize = rect.RB - rect.LT;
						CurFrame.vBackground = nFrame.vSliceSize;
					}
					//���� �������� �̹� �߷� �ִ� ���
					else
					{
						// ���ο� �������� ���� �߰���
						Frame frm = {};

						frm.vLeftTop = rect.LT;
						frm.vSliceSize = rect.RB - rect.LT;
						frm.vBackground = nFrame.vSliceSize;

						anim->AddFrame(frm);
					}
				}
			}
		}
		else
		{
			m_bMouseLeftClick = false;
		}

		if (m_bMouseRightClick)
		{
			ImVec2 mouse_pos_in_canvas(io.MousePos.x, io.MousePos.y);
			Vec2 imageSize(image->GetWidth(), image->GetHeight());
			ImVec2 endPos = mouse_pos_in_canvas;
			endPos -= windowPos;
			endPos -= LeftTop;
			ImVec2 endUV = ImVec2(endPos.x / transformedImageSize.x, endPos.y / transformedImageSize.y);
			m_MouseReleasePixel = Vec2(endUV.x * imageSize.x, endUV.y * imageSize.y);
			m_MouseReleasePixel = Vec2((int)m_MouseReleasePixel.x, (int)m_MouseReleasePixel.y);

			if (m_MouseReleasePixel.x > imageSize.x)
			{
				m_MouseReleasePixel.x = imageSize.x;
			}
			if (m_MouseReleasePixel.y > imageSize.y)
			{
				m_MouseReleasePixel.y = imageSize.y;
			}
		}

		//��Ŭ�� �巡�� ������
		if (0 != m_MouseClickPixel.x && 0 != m_MouseClickPixel.y
			&& 0 != m_MouseReleasePixel.x && 0 != m_MouseReleasePixel.y)
		{
			ImVec2 drawLT = ImVec2(m_MouseClickPixel.x, m_MouseClickPixel.y) * imageDelta;
			ImVec2 drawRB = ImVec2(m_MouseReleasePixel.x, m_MouseReleasePixel.y) * imageDelta;

			drawLT += LeftTop + windowPos;
			drawRB += LeftTop + windowPos;

			ImColor col = ImVec4(1.f, 0.f, 0.f, 1.f);
			draw_list->AddRect(drawLT, drawRB, col);

			// image ����
			D3D11_TEXTURE2D_DESC desc = image->GetDesc();
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.BindFlags = 0;

			Texture* pTex = new Texture;
			pTex->Create(desc.Width, desc.Height, desc.Format, desc.BindFlags, desc.Usage);

			D3D11_MAPPED_SUBRESOURCE data = {};
			Device::GetInst()->GetContext()->CopyResource(pTex->GetTex2D().Get(), image->GetTex2D().Get());
			Device::GetInst()->GetContext()->Map(pTex->GetTex2D().Get(), 0, D3D11_MAP_READ, 0, &data);

			MekeFramesByDrag(m_MouseClickPixel, m_MouseReleasePixel);

			Device::GetInst()->GetContext()->Unmap(pTex->GetTex2D().Get(), 0);
			delete pTex;
		}

		if (!m_bMouseRightClick)
		{
			//�巡�� ���� ���� �̹��� Ȯ���ϱ�
		}

		ImGui::Checkbox("left##TESTLEFT", &m_bMouseLeftClick);
		ImGui::Checkbox("right##TESTRIGHT", &m_bMouseRightClick);
		Vec2 clickPixel(m_MouseClickPixel.x, m_MouseClickPixel.y);
		ImGui::InputFloat2("click##testclick", clickPixel);
		Vec2 endPixel(m_MouseReleasePixel.x, m_MouseReleasePixel.y);
		ImGui::InputFloat2("end##testend", endPixel);

		std::vector<Frame> frames = anim->GetAllFrame();
		std::vector<Frame>::iterator iter = frames.begin();

		//��� ������ �����ͼ� �ʷϻ����� �� �׾��ֱ� / Rect ���� �׷��� �ȿ� �ֱ� ( ���� ���� )
		for (; iter != frames.end(); iter++)
		{
			ImVec2 leftTop = ImVec2(iter->vLeftTop.x, iter->vLeftTop.y) * imageDelta;
			ImVec2 sliceSize = ImVec2(iter->vSliceSize.x, iter->vSliceSize.y) * imageDelta;
			ImVec2 background = ImVec2(iter->vBackground.x, iter->vBackground.y) * imageDelta;
			ImVec2 offset = ImVec2(iter->vOffset.x, iter->vOffset.y) * imageDelta;

			ImColor col = ImVec4(0.f, 1.f, 0.f, 1.f);
			draw_list->AddRect(LeftTop + windowPos + leftTop, LeftTop + windowPos + leftTop + sliceSize, col);

			col = ImVec4(1.f, 1.f, 1.f, 1.f);
			ImVec2 BackgroundLeftTop = leftTop + (sliceSize / 2) - (background / 2);
			draw_list->AddRect(LeftTop + windowPos + BackgroundLeftTop - offset, LeftTop + windowPos + BackgroundLeftTop + background - offset, col);
		}

		//���� ������ ����
		{
			//frame
			Frame curFrame = anim->GetCurFrame();

			ImVec2 leftTop = ImVec2(curFrame.vLeftTop.x, curFrame.vLeftTop.y);
			ImVec2 sliceSize = ImVec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);
			ImVec2 offset = ImVec2(curFrame.vOffset.x, curFrame.vOffset.y);
			ImVec2 background = ImVec2(curFrame.vBackground.x, curFrame.vBackground.y);

			//UV
			ImVec2 ImageSize = ImVec2(image->GetWidth(), image->GetHeight());
			ImVec2 leftTopUV = ImVec2(leftTop.x / ImageSize.x, leftTop.y / ImageSize.y);
			ImVec2 sliceSizeUV = ImVec2(sliceSize.x / ImageSize.x, sliceSize.y / ImageSize.y);
			ImVec2 offsetUV = ImVec2(offset.x / ImageSize.x, offset.y / ImageSize.y);
			ImVec2 backgroundUV = ImVec2(background.x / ImageSize.x, background.y / ImageSize.y);
			ImVec2 rightBotUV = leftTopUV + sliceSizeUV;

			//background in render uv
			ImVec2 BackgroundLeftTopUV = leftTopUV + (sliceSizeUV / 2) - (backgroundUV / 2) - offsetUV;
			ImVec2 BackgroundRightBotUV = BackgroundLeftTopUV + backgroundUV;

			//drawPos
			ImVec2 drawLeftTop = LeftTop + windowPos + maxImageSize + ImVec2(-250, 0);
			ImVec2 drawRightBot = drawLeftTop + ImVec2(100, 100);

			//�ڿ� ��׶��� ��ġ �׸���
			draw_list->AddRectFilled(drawLeftTop, drawRightBot, ImColor(0.3f, 0.3f, 0.3f, 1.f));

			//backgroundUV - LeftTopUV = Delta
			ImVec2 DeltaLeftTopUV = leftTopUV - BackgroundLeftTopUV;
			ImVec2 DeltaLeftTop = ImVec2(DeltaLeftTopUV.x * ImageSize.x, DeltaLeftTopUV.y * ImageSize.y);

			ImVec2 DeltaRightBotUV = BackgroundRightBotUV - rightBotUV;
			ImVec2 DeltaRightBot = ImVec2(DeltaRightBotUV.x * ImageSize.x, DeltaRightBotUV.y * ImageSize.y);

			//final drawPos
			//offset correction
			{
				//lt
				if (DeltaLeftTop.x > 0)
					drawLeftTop.x += DeltaLeftTop.x;
				else
				{
					leftTopUV.x -= DeltaLeftTopUV.x;
				}
				if (DeltaLeftTop.y > 0)
					drawLeftTop.y += DeltaLeftTop.y;
				else
				{
					leftTopUV.y -= DeltaLeftTopUV.y;
				}

				//rb
				if (DeltaRightBot.x > 0)
					drawRightBot.x -= DeltaRightBot.x;
				else
				{
					rightBotUV.x += DeltaRightBotUV.x;
				}
				if (DeltaRightBot.y > 0)
					drawRightBot.y -= DeltaRightBot.y;
				else
				{
					rightBotUV.y += DeltaRightBotUV.y;
				}
			}

			//image Correction
			{
				//lt
				if (drawLeftTop.x > drawRightBot.x)
				{
					float sizeX = drawLeftTop.x - drawRightBot.x;
					float tmpX = drawLeftTop.x;
					drawLeftTop.x = drawRightBot.x;
					drawRightBot.x = tmpX;

					drawLeftTop.x += (sizeX / 2);
					drawRightBot.x -= (sizeX / 2);
				}
				//rb
				if (drawLeftTop.y > drawRightBot.y)
				{
					float sizeY = drawLeftTop.y - drawRightBot.y;
					float tmpY = drawLeftTop.y;
					drawLeftTop.y = drawRightBot.y;
					drawRightBot.y = tmpY;

					drawLeftTop.y += (sizeY / 2);
					drawRightBot.y -= (sizeY / 2);
				}
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

void AnimEditorRenderGUI::MekeFramesByDrag(Vec2 _pixelLT, Vec2 _pixelRB)
{
	//LT ��ġ�� ���İ��� 1�ΰ�� -> ���� �ȼ����� �������� �����Ӹ���� ȣ��
	//LT ��ġ�� ���İ��� 0�ΰ�� -> ��10�ȼ� ��10�ȼ� �� �̵��ϸ鼭 Ȯ�� �� 10�� �ݺ��� ���İ��� 1�� �ȼ��� ã�� ���ϸ� ����
	//ã����� �ݺ�
	//RB ��ġ�� �����ϰų� �Ѿ ��� ����
	int loop = 0;
	while (true)
	{
		break;
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
