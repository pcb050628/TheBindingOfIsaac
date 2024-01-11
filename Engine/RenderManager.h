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

	void Render();
	void Render_Debug();

	void RegisterCamera(Camera* _Cam, int _Idx);
	void RegisterLight2D(Light2D* _light2d);

};

