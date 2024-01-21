#pragma once
#include "GUI.h"

#include <set>

class Texture;
struct Frame;
class AnimEditorGUI;
class AnimEditorRenderGUI :
    public GUI
{
private:
    struct FrameRect
    {
        Vec2 LT;
        Vec2 RB;
    };

private:
    AnimEditorGUI*  m_Target;
    Vec2            m_MouseClickPixel;
    Vec2            m_MouseReleasePixel;
    bool            m_bMouseLeftClick;
    bool            m_bMouseRightClick;

public:
    void RenderUpdate() override;

    void SetTarget(AnimEditorGUI* _inst) { m_Target = _inst; }

private:
    void SizeCheck(ImVec2& _size, ImVec2 _maxSize);
    void MekeFramesByDrag(Vec2 _pixelLT, Vec2 _pixelRB);
    void MakeFrameByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check);
    void GetLeftByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check);
    void GetRightByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check);
    void GetTopByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check);
    void GetBottomByPixelCoord(Vec2 _pixelPos, FrameRect& _frameOut, D3D11_MAPPED_SUBRESOURCE _mappedSub, std::set<Vec2>& _check);

public:
    AnimEditorRenderGUI();
    ~AnimEditorRenderGUI();
};

