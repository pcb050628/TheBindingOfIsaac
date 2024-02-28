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
#include "Texture.h"

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)\
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHICS)
	, m_FOV(0.f)
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck()
{
	m_AspectRatio = Device::GetInst()->GetRenderResolution().x / Device::GetInst()->GetRenderResolution().y;
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
	Vec3 vPos = GetOwner()->GetTransform()->GetRelativePos();
	Matrix matTrans = DirectX::XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	Vec3 vRight = GetOwner()->GetTransform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 vUp = GetOwner()->GetTransform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vFront = GetOwner()->GetTransform()->GetLocalDir(DIR_TYPE::FRONT);

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
		Vec2 vResol = Device::GetInst()->GetRenderResolution();
		m_matProj = DirectX::XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	else if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void Camera::SortObject()
{
	Room* room = ChapterManager::GetInst()->GetCurChapter()->GetRoom(GetOwner()->GetRoomNumber());
	if (!room)
	{
		assert(nullptr);
		return;
	}

	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		const std::vector<GameObject*>& objs 
			= room->GetLayer((LAYER_TYPE)i)->GetGameObject();

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

	Render_PostProcess();
}

void Camera::Render(vector<GameObject*>& _objs)
{
	for (int i = 0; i < _objs.size(); i++)
	{
		_objs[i]->Render();
	}

	_objs.clear();
}

void Camera::Render_PostProcess()
{
	for (int i = 0; i < m_PostProcess.size(); i++)
	{
		RenderManager::GetInst()->CopyRenderTargetToPostProcessTex();
		Texture* pPostProcessTex = RenderManager::GetInst()->GetPostProcessTex();
		pPostProcessTex->UpdateData(13);

		m_PostProcess[i]->Render();
	}

	m_PostProcess.clear();
}

void Camera::SetProjType(PROJ_TYPE _type)
{
	m_ProjType = _type;

	if (PROJ_TYPE::PERSPECTIVE == m_ProjType)
	{
		if (0 == m_FOV)
		{
			m_FOV = 60;
		}
	}
}

bool Camera::GetLayerCheck(int _layerType)
{
	UINT mask = 0;
	mask |= (1 << _layerType);

	return (m_LayerCheck & mask);
}

bool Camera::GetLayerCheck(LAYER_TYPE _layerType)
{
	UINT mask = 0;
	mask |= (1 << (int)_layerType);

	return (m_LayerCheck & mask);
}

void Camera::LayerCheckAll(bool _check)
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		LayerCheck(i, _check);
	}
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

void Camera::SaveToFile(FILE* _file)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _file);
	fwrite(&m_FOV, sizeof(float), 1, _file);
	fwrite(&m_Width, sizeof(float), 1, _file);
	fwrite(&m_Scale, sizeof(float), 1, _file);
	fwrite(&m_AspectRatio, sizeof(float), 1, _file);
	fwrite(&m_Far, sizeof(float), 1, _file);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _file);
}

void Camera::LoadFromFile(FILE* _file)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _file);
	fread(&m_FOV, sizeof(float), 1, _file);
	fread(&m_Width, sizeof(float), 1, _file);
	fread(&m_Scale, sizeof(float), 1, _file);
	fread(&m_AspectRatio, sizeof(float), 1, _file);
	fread(&m_Far, sizeof(float), 1, _file);
	fread(&m_LayerCheck, sizeof(UINT), 1, _file);
}
