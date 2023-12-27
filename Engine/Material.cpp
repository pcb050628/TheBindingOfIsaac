#include "pch.h"
#include "Material.h"

#include "Device.h"
#include "ConstantBuffer.h"

#include "GraphicsShader.h"

Material::Material()
	: Resource(RESOURCE_TYPE::MATERIAL)
	, m_ConstData()
	, m_Textures{}
	, m_Shader()
{
}

Material::~Material()
{
}

bool Material::Load(const std::wstring& _strFilePath)
{
	return false;
}

bool Material::Save()
{
	return false;
}

void Material::UpdateData()
{
	if (m_Shader != nullptr)
		m_Shader->UpdateData();

	ConstantBuffer* cBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	cBuffer->SetData(&m_ConstData);
	cBuffer->UpdateData();
}
