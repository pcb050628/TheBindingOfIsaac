#include "pch.h"
#include "ComputeShader.h"

#include "Device.h"
#include "ConstantBuffer.h"

ComputeShader::ComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ) : Shader(RESOURCE_TYPE::COMPUTE_SHADER)
	, m_ThreadX(_ThreadX)
	, m_ThreadY(_ThreadY)
	, m_ThreadZ(_ThreadZ)
{
}

ComputeShader::~ComputeShader()
{
}

int ComputeShader::Create(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strContentPath = GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0
		, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	if (FAILED(Device::GetInst()->GetDevice()->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf())))
		return E_FAIL;

    return S_OK;
}

void ComputeShader::Execute()
{
	if (FAILED(UpdateData()))
		return;

	static ConstantBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_Const);
	pCB->UpdateData_CS();

	// 컴퓨트 쉐이더 실행
	Device::GetInst()->GetContext()->CSSetShader(m_CS.Get(), 0, 0);
	Device::GetInst()->GetContext()->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	Clear();
}