#include "pch.h"
#include "Camera.h"

#include "Device.h"
#include "ChapterManager.h"
#include "RenderManager.h"

#include "Layer.h"

#include "GameObject.h"

#include "Transform.h"
#include "RenderComponent.h"

#include "Material.h"
#include "GraphicsShader.h"

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)\
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHICS)
	, m_FOV(0.f)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck()
{
	m_AspectRatio = Device::GetInst()->GetResolution().x / Device::GetInst()->GetResolution().y;
}

Camera::~Camera()
{
}

void Camera::Update()
{
}

void Camera::LateUpdate()
{
	m_matView = DirectX::XMMatrixIdentity();
	m_matProj = DirectX::XMMatrixIdentity();

	//
	Vec3 vPos = GetOwner()->GetComponent<Transform>()->GetRelativePos();
	Matrix matTrans = DirectX::XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	Vec3 vRight = GetOwner()->GetComponent<Transform>()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 vUp = GetOwner()->GetComponent<Transform>()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vFront = GetOwner()->GetComponent<Transform>()->GetLocalDir(DIR_TYPE::FRONT);

	//
	Matrix matRot = DirectX::XMMatrixIdentity();

	matRot._11 = vRight.x; matRot._12 = vUp.x; matRot._13 = vFront.x;
	matRot._21 = vRight.y; matRot._22 = vUp.y; matRot._23 = vFront.y;
	matRot._31 = vRight.z; matRot._32 = vUp.z; matRot._33 = vFront.z;

	//
	m_matView = matTrans * matRot;

	//
	if (m_ProjType == PROJ_TYPE::ORTHOGRAPHICS)
	{
		Vec2 vResol = Device::GetInst()->GetResolution();
		m_matProj = DirectX::XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	else if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void Camera::SortObject()
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		const std::vector<GameObject*>& objs 
			= ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetLayer((LAYER_TYPE)i)->GetGameObject();

		for (int j = 0; j < objs.size(); j++)
		{
			if (!( objs[j]->GetRenderCom()
				&& objs[j]->GetRenderCom()->GetMesh()
				&& objs[j]->GetRenderCom()->GetMaterial()
				&& objs[j]->GetRenderCom()->GetMaterial()->GetShader()))
			{
				continue;
			}

			SHADER_DOMAIN domain = objs[j]->GetRenderCom()->GetMaterial()->GetShader()->GetDomain();

			switch (domain)
			{
			case SHADER_DOMAIN::DOMAIN_OPAUQUE:
				m_Opaque.push_back(objs[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_Masked.push_back(objs[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
				m_Transparent.push_back(objs[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_PostProcess.push_back(objs[j]);
				break;
			}
		}
	}
}

void Camera::Render()
{
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;

	Render(m_Opaque);
	Render(m_Masked);
	Render(m_Transparent);
	Render(m_PostProcess);
}

void Camera::Render(std::vector<GameObject*>& _objs)
{
	for (int i = 0; i < _objs.size(); i++)
	{
		_objs[i]->Render();
	}

	_objs.clear();
}

void Camera::LayerCheck(int _layerIdx, bool _check)
{
	if (_check)
	{
		m_LayerCheck |= (1 << _layerIdx);
	}
	else
	{
		m_LayerCheck &= ~(1 << _layerIdx);
	}
}

void Camera::LayerCheck(LAYER_TYPE _layerType, bool _check)
{
	if (_check)
	{
		m_LayerCheck |= (1 << (int)_layerType);
	}
	else
	{
		m_LayerCheck &= ~(1 << (int)_layerType);
	}
}

void Camera::SetCameraPriority(int _Priority)
{
	RenderManager::GetInst()->RegisterCamera(this, _Priority);
}
