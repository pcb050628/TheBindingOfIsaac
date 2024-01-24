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

		//현재 아틀라스 그리기 / Rect 같이 그려서 안에 넣기 ( 아직 안함 )
		ImVec2 maxImageSize = ImGui::GetWindowSize();
		maxImageSize.y /= 2;
		ImVec2 windowPos = ImGui::GetWindowPos() + ImVec2(0, 50);
		ImVec2 LeftTop = ImVec2(100, 0);
		ImVec2 transformedImageSize = ImVec2(image->GetWidth(), image->GetHeight());

		SizeCheck(transformedImageSize, maxImageSize);
		draw_list->AddImage(image->GetSRV().Get(), LeftTop + windowPos, LeftTop + transformedImageSize + windowPos);

		ImVec2 imageDelta = ImVec2(transformedImageSize.x / image->GetWidth(), transformedImageSize.y / image->GetHeight());

		//마우스 클릭으로 이미지 프레임 만들기
		ImGui::InvisibleButton("canvas", LeftTop + transformedImageSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

		//우클릭
		if (ImGui::IsKeyDown(ImGuiKey_MouseRight))
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				//마우스 위치 얻기
				//마우스 우클릭이 처음 일어났을 때
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


		//좌클릭
		if (ImGui::IsKeyDown(ImGuiKey_MouseLeft))
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				m_bMouseLeftClick = true;
				//마우스 위치 얻기
				ImVec2 mouse_pos_in_canvas(io.MousePos.x, io.MousePos.y);
				mouse_pos_in_canvas -= windowPos;
				mouse_pos_in_canvas -= LeftTop;

				//mosuePos -> PixelPos
				//UV값 얻고 UV값으로 픽셀값 얻기
				ImVec2 UV = ImVec2(mouse_pos_in_canvas.x / transformedImageSize.x, mouse_pos_in_canvas.y / transformedImageSize.y); //ImVec2(mousePosInImage.x / transformedImageSize.x, mousePosInImage.y / transformedImageSize.y);
				Vec2 Pixel = Vec2(UV.x * image->GetWidth(), UV.y * image->GetHeight());
				Pixel.x = (int)Pixel.x;
				Pixel.y = (int)Pixel.y;

				//현재 픽셀위치가 lefttop + sliceSize 안 위치에 해당하는 프레임이 있는지 검사
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

				//픽셀위치에 해당하는 프레임이 있다면 그 프레임을 현재 프레임으로 바꿈
				if (IsExist)
				{
					anim->SetCurFrameIdx(idx);
				}
				//없으면 픽셀위치로 프레임 생성 또는 프레임 정보 설정
				else
				{
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
						//다음 프레임이 존재하고
						bool check = true;
						int nextIdx = anim->GetCurFrameIdx() + 1;
						while (nextIdx < anim->GetMaxFrameIdx())
						{
							//다음 프레임이 잘려있지않다면 
							Frame& nFrm = anim->GetFrame(nextIdx);
							if (0 == nFrm.vSliceSize.x || 0 == nFrm.vSliceSize.y)
							{
								//다음 프레임을 설정하고 다음 프레임을 현재 프레임으로 변경함
								nFrm.vLeftTop = rect.LT;
								nFrm.vSliceSize = rect.RB - rect.LT;
								nFrm.vBackground = nFrame.vSliceSize;
								anim->SetCurFrameIdx(nextIdx);
								break;
							}
							else
							{
								nextIdx++;
							}
						}

						if (!(nextIdx < anim->GetMaxFrameIdx()))
						{
							check = false;
						}

						//새로운 프레임을 만들어서 추가함
						if (!check)
						{
							Frame frm = {};

							frm.vLeftTop = rect.LT;
							frm.vSliceSize = rect.RB - rect.LT;
							frm.vBackground = nFrame.vSliceSize;

							anim->AddFrame(frm);
						}
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

		//우클릭 드래그 중 사각형 띄우기
		if (0 != m_MouseClickPixel.x && 0 != m_MouseClickPixel.y
			&& 0 != m_MouseReleasePixel.x && 0 != m_MouseReleasePixel.y)
		{
			ImVec2 drawLT = ImVec2(m_MouseClickPixel.x, m_MouseClickPixel.y) * imageDelta;
			ImVec2 drawRB = ImVec2(m_MouseReleasePixel.x, m_MouseReleasePixel.y) * imageDelta;

			drawLT += LeftTop + windowPos;
			drawRB += LeftTop + windowPos;

			ImColor col = ImVec4(1.f, 0.f, 0.f, 1.f);
			draw_list->AddRect(drawLT, drawRB, col);
		}

		//우클릭을 때었을 때
		if (!m_bMouseRightClick
			&& 0 != m_MouseClickPixel.x   && 0 != m_MouseClickPixel.y
			&& 0 != m_MouseReleasePixel.x && 0 != m_MouseReleasePixel.y)
		{
			//MakeFramesByDrag 함수에 쓰일 Out
			std::vector<Frame> outFrames;

			// image 복사
			D3D11_TEXTURE2D_DESC desc = image->GetDesc();
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.BindFlags = 0;
			
			Texture* pTex = new Texture;
			pTex->Create(desc.Width, desc.Height, desc.Format, desc.BindFlags, desc.Usage);
			
			D3D11_MAPPED_SUBRESOURCE data = {};
			Device::GetInst()->GetContext()->CopyResource(pTex->GetTex2D().Get(), image->GetTex2D().Get());
			Device::GetInst()->GetContext()->Map(pTex->GetTex2D().Get(), 0, D3D11_MAP_READ, 0, &data);
			
			//프레임 만들기
			MakeFramesByDrag(m_MouseClickPixel, m_MouseReleasePixel, data, outFrames);
			m_MouseClickPixel = Vec2(0.f);
			m_MouseReleasePixel = Vec2(0.f);
			
			//이미지 해제
			Device::GetInst()->GetContext()->Unmap(pTex->GetTex2D().Get(), 0);
			delete pTex;

			//outFrame 정보를 AnimFrame으로 옮김
			std::vector<Frame>& animFrame = anim->GetAllFrame();
			int idx = 0;
			for (size_t i = 0; i < outFrames.size(); i++)
			{
				bool animfrmcheck = false;
				//animFrame의 프레임 정보 확인
				if (idx < animFrame.size())
				{
					while (true)
					{
						if (!(idx < animFrame.size()))
						{
							animfrmcheck = true;
							break;
						}

						//SliceSize 의 x 또는 y 의 정보가 0일 경우 만들어진 프레임이 아니라고 판단하여 정보를 그 프레임에 덮어 씌움
						if (0 == animFrame[idx].vSliceSize.x || 0 == animFrame[idx].vSliceSize.y)
						{
							animFrame[idx] = outFrames[i];
							break;
						}

						idx++;
					}
				}

				//AnimFrame의 모든 정보가 채워져있을 경우
				if(animfrmcheck)
				{
					animFrame.push_back(outFrames[i]);
				}
			}
		}

		ImGui::Checkbox("left##TESTLEFT", &m_bMouseLeftClick);
		ImGui::Checkbox("right##TESTRIGHT", &m_bMouseRightClick);
		Vec2 clickPixel(m_MouseClickPixel.x, m_MouseClickPixel.y);
		ImGui::InputFloat2("click##testclick", clickPixel);
		Vec2 endPixel(m_MouseReleasePixel.x, m_MouseReleasePixel.y);
		ImGui::InputFloat2("end##testend", endPixel);

		std::vector<Frame> frames = anim->GetAllFrame();
		std::vector<Frame>::iterator iter = frames.begin();

		//모든 프레임 가져와서 초록색으로 선 그어주기, 현재 프레임일 경우 빨간색으로 선 그어주기
		{
			int idx = 0;
			for (; iter != frames.end(); iter++, idx++)
			{
				ImVec2 leftTop = ImVec2(iter->vLeftTop.x, iter->vLeftTop.y) * imageDelta;
				ImVec2 sliceSize = ImVec2(iter->vSliceSize.x, iter->vSliceSize.y) * imageDelta;
				ImVec2 background = ImVec2(iter->vBackground.x, iter->vBackground.y) * imageDelta;
				ImVec2 offset = ImVec2(iter->vOffset.x, iter->vOffset.y) * imageDelta;

				ImColor col = ImVec4(0.f, 1.f, 0.f, 1.f);
				if (idx == anim->GetCurFrameIdx())
					col = ImVec4(1.f, 0.f, 0.f, 1.f);
				draw_list->AddRect(LeftTop + windowPos + leftTop, LeftTop + windowPos + leftTop + sliceSize, col);

				col = ImVec4(1.f, 1.f, 1.f, 1.f);
				ImVec2 BackgroundLeftTop = leftTop + (sliceSize / 2) - (background / 2);
				draw_list->AddRect(LeftTop + windowPos + BackgroundLeftTop - offset, LeftTop + windowPos + BackgroundLeftTop + background - offset, col);
			}
		}

		//현재 프레임 띄우기
		{
			//frame
			Frame curFrame = anim->GetCurFrame();

			ImVec2 leftTop = ImVec2(curFrame.vLeftTop.x, curFrame.vLeftTop.y);
			ImVec2 sliceSize = ImVec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);
			ImVec2 offset = ImVec2(curFrame.vOffset.x, curFrame.vOffset.y);
			ImVec2 background = ImVec2(curFrame.vBackground.x, curFrame.vBackground.y);
			ImVec2 rightBot = leftTop + sliceSize;

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

			//뒤에 백그라운드 위치 그리기
			draw_list->AddRectFilled(drawLeftTop, drawRightBot, ImColor(0.3f, 0.3f, 0.3f, 1.f));

			//backgroundUV - LeftTopUV = Delta
			ImVec2 DeltaLeftTopUV = leftTopUV - BackgroundLeftTopUV;
			ImVec2 DeltaLeftTop = ImVec2(DeltaLeftTopUV.x * ImageSize.x, DeltaLeftTopUV.y * ImageSize.y);
			//DeltaLeftTop *= imageDelta;

			ImVec2 DeltaRightBotUV = BackgroundRightBotUV - rightBotUV;
			ImVec2 DeltaRightBot = ImVec2(DeltaRightBotUV.x * ImageSize.x, DeltaRightBotUV.y * ImageSize.y);
			//DeltaRightBot *= imageDelta;

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

void AnimEditorRenderGUI::MakeFramesByDrag(Vec2 _pixelLT, Vec2 _pixelRB, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::vector<Frame>& _Out)
{
	//LT 위치의 알파값이 1인경우 -> 현재 픽셀값을 기준으로 프레임만들기 호출
	//LT 위치의 알파값이 0인경우 -> 우10픽셀 하10픽셀 씩 이동하면서 확인 총 10번 반복후 알파값이 1인 픽셀을 찾지 못하면 리턴
	//찾은경우 Out에 푸시
	//반복
	//RB 위치의 도달하거나 넘어간 경우 리턴

	Vec2 LT = _pixelLT;
	Vec2 RB = _pixelLT;

	int IncreaseXUnit = (_pixelRB - _pixelLT).x / 5;
	int IncreaseYUnit = (_pixelRB - _pixelLT).y / 5;

	//프레임을 MakeFrameByPixelCoord 호출 할 때 필요한 변수 선언
	FrameRect outFrameRect;
	std::set<Vec2> check;

	int x = (int)LT.x; // Pixel x-coordinate
	int y = (int)LT.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch; // row pitch 한 행의 간격, image->width * 픽셀의 크기 와 같을 것
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4); // 데이터 자체가 주소값이기 때문에 값을 더해서 원하는 주소(픽셀)로 이동한다, (y + rowPitch) 행 만큼 이동, (x * 4) 픽셀의 단위만큼(열) 이동

	// Access RGBA values
	BYTE alpha = pData[3];

	int count = 0;
	int loop = 0;
	bool xyIncreaseValue = false;
	while (loop < 50)
	{
		//픽셀의 위치가 최대값에 도달했거나 넘었다면 리턴
		if (LT.y >= _pixelRB.y /*||  RB.y >= _pixelRB.y*/)
			return;

		if (xyIncreaseValue)
		{
			if (count > 5) 
			{ 
				loop++; 
				count = 0; 
				xyIncreaseValue = false;
			}
		}
		else
		{
			if (LT.x >= _pixelRB.x)
			{
				loop++; 
				count = 0; 
				xyIncreaseValue = true;
				LT.x = _pixelLT.x; 
			}
		}

		

		//픽셀 데이터 확인
		x = (int)LT.x;
		y = (int)LT.y;

		pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4);
		alpha = pData[3];

		//알파가 0이 아닐 때(비어있는 픽셀이 아닐 떄)
		if (alpha)
		{
			//픽셀 위치가 _out에 있는지 확인 있다면 그 부분에 다시 프레임을 만들면 안됨
			bool OutCheck = false;

			if (_Out.size() > 0)
			{
				for (int idx = 0; idx < _Out.size(); idx++)
				{
					Vec2 _outlt = _Out[idx].vLeftTop;
					Vec2 _outrb = _Out[idx].vLeftTop + _Out[idx].vSliceSize;

					if (LT.x > _outlt.x && LT.x < _outrb.x
						&& LT.y > _outlt.y && LT.y < _outrb.y)
					{
						OutCheck = true;
						break;
					}
				}
			}	

			if (!OutCheck)
			{
				//계속 사용될 변수 초기화
				outFrameRect.LT = LT;
				outFrameRect.RB = LT;
				MakeFrameByPixelCoord(LT, outFrameRect, _mappedSub, check);
				//함수 내에서 위치가 겹치는 것을 피하기 위해 필요한 변수이기 때문에 함수에서 나오면 바로 초기화해도 문제가없음
				check.clear();

				//새로운 프레임 푸시
				Frame frm;
				frm.vLeftTop = outFrameRect.LT;
				frm.vSliceSize = outFrameRect.RB - outFrameRect.LT;
				frm.vBackground = frm.vSliceSize;
				_Out.push_back(frm);

				//좌상단을 가장 최근에 만들어진 프레임의 가장 우측부분으로 설정
				LT = Vec2(outFrameRect.RB.x, outFrameRect.LT.y);
				//우하단을 가장 최근에 만들어진 프레임의 우하단으로 설정
				RB = outFrameRect.RB;

				//카운트 초기화
				count = 0;
				xyIncreaseValue = false;
			}
			else
			{
				//카운트 증가
				count++;
				//좌상단(프레임을 만들기 시작할 위치) 증가(우하단으로 1픽셀씩 이동)
				if (!xyIncreaseValue)
					LT.x += IncreaseXUnit;
				else
					LT.y += IncreaseYUnit;
			}
		}
		//알파가 0이라면
		else
		{
			//카운트 증가
			count++;
			//좌상단(프레임을 만들기 시작할 위치) 증가(우하단으로 1픽셀씩 이동)
			if(!xyIncreaseValue)
				LT.x += IncreaseXUnit;
			else
				LT.y += IncreaseYUnit;
		}
	}
}

void AnimEditorRenderGUI::MakeFrameByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check)
{
	int x = (int)_pixelPos.x; // Pixel x-coordinate
	int y = (int)_pixelPos.y; // Pixel y-coordinate

	if (x < 0 || y < 0 || x >= m_Target->m_EditAnim->GetAtlas()->GetWidth() || y >= m_Target->m_EditAnim->GetAtlas()->GetHeight())
		return;

	// Assuming a 32-bit RGBA format, calculate the offset
	UINT rowPitch = _mappedSub.RowPitch; // row pitch 한 행의 간격, image->width * 픽셀의 크기 와 같을 것
	BYTE* pData = static_cast<BYTE*>(_mappedSub.pData) + (y * rowPitch) + (x * 4); // 데이터 자체가 주소값이기 때문에 값을 더해서 원하는 주소(픽셀)로 이동한다

	// Access RGBA values
	BYTE alpha = pData[3];

	if (!alpha)
	{
		for (int i = 0; i < 30; i++)
		{
			x += 1;

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
