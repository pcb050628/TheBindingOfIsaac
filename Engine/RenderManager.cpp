#include "pch.h"
#include "RenderManager.h"

#include "Device.h"
#include "StructuredBuffer.h"

#include "Time.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Light2D.h"

RenderManager::RenderManager()
	: m_Cams{}
	, m_DebugObj(nullptr)
	, m_DebugShapeInfos{}
{

}

RenderManager::~RenderManager()
{

}

void RenderManager::Init()
{
	m_Light2DBuffer = new StructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

	m_DebugObj = new GameObject;
	m_DebugObj->AddComponent(new Transform);
	m_DebugObj->AddComponent(new MeshRenderer);

	m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"")); 
	m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"")); 
}

void RenderManager::Update()
{
	Device::GetInst()->DrawStart();

	UpdateData();

	Render();
	Render_Debug();

	Clear();

	Device::GetInst()->DrawEnd();
}

void RenderManager::UpdateData()
{
	static std::vector<tLightInfo> infos = {};
	for (int i = 0; i < m_Light2D.size(); i++)
	{
		infos.push_back(m_Light2D[i]->GetLightInfo());
	}

	m_Light2DBuffer->SetData(infos.data(), infos.size());
	m_Light2DBuffer->UpdateData(11);

	infos.clear();
}

void RenderManager::Clear()
{
	m_Light2D.clear();
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
			m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"debug_Material"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"CircleMesh_Debug"));
			m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"debug_Material"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"CubeMesh"));
			m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"debug_Material"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_DebugObj->GetComponent<MeshRenderer>()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"SphereMesh"));
			m_DebugObj->GetComponent<MeshRenderer>()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"debug_Material"));
			break;
		}

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

void RenderManager::RegisterLight2D(Light2D* _light2d)
{
	m_Light2D.push_back(_light2d);
}
