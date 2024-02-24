#pragma once

class StructuredBuffer;

class GameObject;
class Camera;
class Light2D;

class RenderManager
{
	SINGLETON(RenderManager)
private:
	std::vector<Camera*>			m_Cams;
	std::vector<Light2D*>			m_Light2D;

	class Texture*					m_PostProcessTex;

	StructuredBuffer*				m_Light2DBuffer;

	std::vector<tDebugShapeInfo>	m_DebugShapeInfos;
	GameObject*						m_DebugObj;

public:
	void Init();
	void Update();

	void UpdateData();
	void Clear();

	void AddDebugShapeInfo(tDebugShapeInfo& _info) { m_DebugShapeInfos.push_back(_info); }

	void Render();
	void Render_Debug();
	void EditRender(Camera* _editCam);

	void RegisterCamera(Camera* _Cam, int _Idx);
	void RegisterLight2D(Light2D* _light2d);

	void CopyRenderTargetToPostProcessTex();
	Texture* GetPostProcessTex() { return m_PostProcessTex; }

};

