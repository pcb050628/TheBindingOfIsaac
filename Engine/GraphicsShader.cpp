#include "pch.h"
#include "GraphicsShader.h"

#include "Device.h"

GraphicsShader::GraphicsShader() : Shader(RESOURCE_TYPE::GRAPHICS_SHADER)
	, m_pVSBlob(nullptr)
	, m_pHSBlob(nullptr)
	, m_pDSBlob(nullptr)
	, m_pGSBlob(nullptr)
	, m_pPSBlob(nullptr)
	, m_pVertexShader(nullptr)
	, m_pHullShader(nullptr)
	, m_pDomainShader(nullptr)
	, m_pGeometryShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_VSPath(L"NULL")
	, m_HSPath(L"NULL")
	, m_DSPath(L"NULL")
	, m_GSPath(L"NULL")
	, m_PSPath(L"NULL")
	, m_VSFuncName("NULL")
	, m_HSFuncName("NULL")
	, m_DSFuncName("NULL")
	, m_GSFuncName("NULL")
	, m_PSFuncName("NULL")
	, m_Topology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

GraphicsShader::~GraphicsShader()
{
}

int GraphicsShader::CreateVertexShader(const std::wstring& _strRelativePath, const std::string& _strFuncName)
{
	std::wstring filePath = GetContentPath() + _strRelativePath;

	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_pVSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf())))
	{
		if (nullptr != m_pErrBlob)
		{
			char* pErrMsg = (char*)m_pErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}
		return E_FAIL;
	}

	Device::GetInst()->GetDevice()->CreateVertexShader(
		m_pVSBlob.Get(), m_pVSBlob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());

	D3D11_INPUT_ELEMENT_DESC desc[3] = {};

	desc[0].InputSlot = 0;
	desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[0].SemanticName = "POSITION";
	desc[0].SemanticIndex = 0;
	desc[0].InstanceDataStepRate = 0;
	desc[0].AlignedByteOffset = 0;
	desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	desc[1].InputSlot = 0;
	desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[1].SemanticName = "COLOR";
	desc[1].SemanticIndex = 0;
	desc[1].InstanceDataStepRate = 0;
	desc[1].AlignedByteOffset = 12;
	desc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	desc[2].InputSlot = 0;
	desc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc[2].SemanticName = "TEXCOORD";
	desc[2].SemanticIndex = 0;
	desc[2].InstanceDataStepRate = 0;
	desc[2].AlignedByteOffset = 28;
	desc[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	Device::GetInst()->GetDevice()->CreateInputLayout(
		desc, 3, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), m_pInputLayout.GetAddressOf());

	return S_OK;
}

int GraphicsShader::CreateHullShader(const std::wstring& _strRelativePath, const std::string& _strFuncName)
{
	return 0;
}

int GraphicsShader::CreateDomainShader(const std::wstring& _strRelativePath, const std::string& _strFuncName)
{
	return 0;
}

int GraphicsShader::CreateGeometryShader(const std::wstring& _strRelativePath, const std::string& _strFuncName)
{
	return 0;
}

int GraphicsShader::CreatePixelShader(const std::wstring& _strRelativePath, const std::string& _strFuncName)
{
	std::wstring filePath = GetContentPath() + _strRelativePath;

	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_pPSBlob.GetAddressOf(), m_pErrBlob.GetAddressOf())))
	{
		if (nullptr != m_pErrBlob)
		{
			char* pErrMsg = (char*)m_pErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}
		return E_FAIL;
	}

	Device::GetInst()->GetDevice()->CreatePixelShader(
		m_pPSBlob.Get(), m_pPSBlob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());

	return S_OK;
}

bool GraphicsShader::Load(std::wstring _path)
{
	filesystem::path filePath = _path;
	std::wifstream fileStream(filePath);

	if (fileStream.is_open())
	{
		std::wstring line;

		while (true)
		{
			std::getline(fileStream, line);

			if (line == L"END")
				break;
			else
			{
				if (line == L"[VS_PATH]")
				{
					std::getline(fileStream, line);
					m_VSPath = line;
				}
				else if(line == L"[HS_PATH]")
				{
					std::getline(fileStream, line);
					m_HSPath = line;
				}
				else if (line == L"[DS_PATH]")
				{
					std::getline(fileStream, line);
					m_DSPath = line;
				}
				else if (line == L"[GS_PATH]")
				{
					std::getline(fileStream, line);
					m_GSPath = line;
				}
				else if (line == L"[PS_PATH]")
				{
					std::getline(fileStream, line);
					m_PSPath = line;
				}
			}
		}
	}

	if (m_VSPath != L"NULL" && m_VSFuncName != "NULL")
		CreateVertexShader(m_VSPath, m_VSFuncName);
	if (m_HSPath != L"NULL" && m_HSFuncName != "NULL")
		CreateHullShader(m_HSPath, m_HSFuncName);
	if (m_DSPath != L"NULL" && m_DSFuncName != "NULL")
		CreateDomainShader(m_DSPath, m_DSFuncName);
	if (m_GSPath != L"NULL" && m_GSFuncName != "NULL")
		CreateGeometryShader(m_GSPath, m_GSFuncName);
	if (m_PSPath != L"NULL" && m_PSFuncName != "NULL")
		CreatePixelShader(m_PSPath, m_PSFuncName);

	return true;
}

bool GraphicsShader::Save()
{
	filesystem::path filePath = GetContentPath() + L"Resource\\Shader\\Graphics\\" + m_ResourceName;
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		fileStream << L"[VS_PATH]\n" << m_VSPath.c_str() << std::endl;
		fileStream << L"[VS_FuncName]\n" << m_VSFuncName.c_str() << std::endl;

		fileStream << L"[HS_PATH]\n" << m_HSPath.c_str() << std::endl;
		fileStream << L"[HS_FuncName]\n" << m_HSFuncName.c_str() << std::endl;

		fileStream << L"[DS_PATH]\n" << m_DSPath.c_str() << std::endl;
		fileStream << L"[DS_FuncName]\n" << m_DSFuncName.c_str() << std::endl;

		fileStream << L"[GS_PATH]\n" << m_GSPath.c_str() << std::endl;
		fileStream << L"[GS_FuncName]\n" << m_GSFuncName.c_str() << std::endl;

		fileStream << L"[PS_PATH]\n" << m_PSPath.c_str() << std::endl;
		fileStream << L"[PS_FuncName]\n" << m_PSFuncName.c_str() << std::endl;

		fileStream << L"END";

		fileStream.close();
		
		return true;
	}
	else
		return false;
}

void GraphicsShader::UpdateData()
{
	Device::GetInst()->GetContext()->IASetInputLayout(m_pInputLayout.Get());
	Device::GetInst()->GetContext()->IASetPrimitiveTopology(m_Topology);

	Device::GetInst()->GetContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->HSSetShader(m_pHullShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->DSSetShader(m_pDomainShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}
