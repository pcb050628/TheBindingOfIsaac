#pragma once

class RenderManager
{
	SINGLETON(RenderManager)
private:
	std::vector<class Camera*> m_Cams;

	std::vector<tDebugShapeInfo> m_DebugShapeInfos;

	class GameObject* m_DebugObj;

public:
	void Init();
	void Update();

	void Render();
	void Render_Debug();

	void RegisterCamera(Camera* _Cam, int _Idx);

};

