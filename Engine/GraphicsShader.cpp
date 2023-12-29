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
	, m_Topology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_NONE)
	, m_DSSType(DSS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
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

	if (FAILED(Device::GetInst()->GetDevice()->CreateVertexShader(
		m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf())))
		return E_FAIL;

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

	if (FAILED(Device::GetInst()->GetDevice()->CreateInputLayout(
		desc, 3, m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), m_pInputLayout.GetAddressOf())))
		return E_FAIL;

	m_VSPath = _strRelativePath;
	m_VSFuncName = _strFuncName;

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

	if (FAILED(Device::GetInst()->GetDevice()->CreatePixelShader(
		m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf())))
		return E_FAIL;

	m_PSPath = _strRelativePath;
	m_PSFuncName = _strFuncName;

	return S_OK;
}

bool GraphicsShader::Load(const std::wstring& _strFilePath)
{
	filesystem::path filePath = _strFilePath;
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	m_ResourceName = szName;
	m_ResourcePath = _strFilePath;

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
				else if (line == L"[VS_FuncName]")
				{
					std::getline(fileStream, line);
					m_VSFuncName = std::string().assign(line.begin(), line.end());
				}
				else if(line == L"[HS_PATH]")
				{
					std::getline(fileStream, line);
					m_HSPath = line;
				}
				else if (line == L"[HS_FuncName]")
				{
					std::getline(fileStream, line);
					m_HSFuncName = std::string().assign(line.begin(), line.end());
				}
				else if (line == L"[DS_PATH]")
				{
					std::getline(fileStream, line);
					m_DSPath = line;
				}
				else if (line == L"[DS_FuncName]")
				{
					std::getline(fileStream, line);
					m_DSFuncName = std::string().assign(line.begin(), line.end());
				}
				else if (line == L"[GS_PATH]")
				{
					std::getline(fileStream, line);
					m_GSPath = line;
				}
				else if (line == L"[GS_FuncName]")
				{
					std::getline(fileStream, line);
					m_GSFuncName = std::string().assign(line.begin(), line.end());
				}
				else if (line == L"[PS_PATH]")
				{
					std::getline(fileStream, line);
					m_PSPath = line;
				}
				else if (line == L"[PS_FuncName]")
				{
					std::getline(fileStream, line);
					m_PSFuncName = std::string().assign(line.begin(), line.end());
				}

				else if (line == L"[RS_TYPE]")
				{
					std::getline(fileStream, line);
					m_RSType = (RS_TYPE)std::stoi(line);
				}
				else if (line == L"[DSS_TYPE]")
				{
					std::getline(fileStream, line);
					m_DSSType = (DSS_TYPE)std::stoi(line);
				}
				else if (line == L"[BS_TYPE]")
				{
					std::getline(fileStream, line);
					m_BSType = (BS_TYPE)std::stoi(line);
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
	filePath += ".txt";
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

		fileStream << L"[RS_TYPE]\n" << (UINT)m_RSType << std::endl;
		fileStream << L"[DSS_TYPE]\n" << (UINT)m_DSSType << std::endl;
		fileStream << L"[BS_TYPE]\n" << (UINT)m_BSType << std::endl;

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

	Device::GetInst()->GetContext()->RSSetState(Device::GetInst()->GetRasterizerState(m_RSType).Get());
	Device::GetInst()->GetContext()->OMSetDepthStencilState(Device::GetInst()->GetDepthStencilState(m_DSSType).Get(), 0);
	Device::GetInst()->GetContext()->OMSetBlendState(Device::GetInst()->GetBlendState(m_BSType).Get(), nullptr, 0xffffffff);

	Device::GetInst()->GetContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->HSSetShader(m_pHullShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->DSSetShader(m_pDomainShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
	Device::GetInst()->GetContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}
