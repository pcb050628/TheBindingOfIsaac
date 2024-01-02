#include "pch.h"
#include "RenderManager.h"

#include "Device.h"
#include "Time.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"

RenderManager::RenderManager()
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::Init()
{
	m_DebugObj = new GameObject;
	m_DebugObj->AddComponent(new Transform);
	m_DebugObj->AddComponent(new MeshRenderer);

	m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"")); 
	m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"")); 
}

void RenderManager::Update()
{
	Device::GetInst()->DrawStart();

	Render();
	Render_Debug();

	Device::GetInst()->DrawEnd();
}

void RenderManager::Render()
{
	for (int i = 0; i < m_Cams.size(); i++)
	{
		if(m_Cams[i] != nullptr)
			m_Cams[i]->Render();
	}
}

void RenderManager::Render_Debug()
{
	auto iter = m_DebugShapeInfos.begin();

	for (; iter != m_DebugShapeInfos.end();)
	{
		switch ((*iter).eShape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"CubeMesh"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"SphereMesh"));
			break;
		}

		m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"debug_Material"));
		m_DebugObj->GetComponent<MeshRenderer>()->GetMaterial()->SetScalarParam((*iter).vColor, VEC4_0);

		m_DebugObj->GetComponent<Transform>()->SetWorldMat((*iter).matWorld);
		m_DebugObj->GetComponent<Transform>()->UpdateData();

		m_DebugObj->Render();

		(*iter).fDuration += Time::GetInst()->GetDeltaTime();
		if ((*iter).fDuration <= (*iter).fLifeTime)
			iter = m_DebugShapeInfos.erase(iter);
		else
			iter++;
	}
}

void RenderManager::RegisterCamera(Camera* _Cam, int _Idx)
{
	if (m_Cams.size() <= _Idx + 1)
	{
		m_Cams.resize(_Idx + 1);
	}

	// 동일한 우선순위의 카메라가 이미 존재하면 assert
	assert(nullptr == m_Cams[_Idx]);

	m_Cams[_Idx] = _Cam;
}
