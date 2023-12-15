#include "pch.h"
#include "ConstantBuffer.h"

#include "Device.h"

ConstantBuffer::ConstantBuffer(CB_TYPE _type)
	: m_Type(_type)
	, m_ElementCount()
	, m_ElementSize()
	, m_Desc()
{
}

ConstantBuffer::~ConstantBuffer()
{
}

int ConstantBuffer::Create(UINT _Size, UINT _Count)
{
	m_ElementCount = _Count;
	m_ElementSize = _Size;

	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	m_Desc.Usage = D3D11_USAGE_DYNAMIC; 

	if (FAILED(Device::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		MessageBoxW(nullptr, L"ConstBuffer 생성 실패", L"실패", MB_OK);
		return E_FAIL;
	}

    return S_OK;
}

void ConstantBuffer::SetData(void* _Src, UINT _ElementCount)
{
	if (_ElementCount == 0)
		_ElementCount = m_ElementCount;

	D3D11_MAPPED_SUBRESOURCE tSub = {};

	Device::GetInst()->GetContext()->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _Src, m_ElementSize * _ElementCount);
	Device::GetInst()->GetContext()->Unmap(m_CB.Get(), 0);
}

void ConstantBuffer::UpdateData()
{
	Device::GetInst()->GetContext()->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	Device::GetInst()->GetContext()->HSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	Device::GetInst()->GetContext()->DSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	Device::GetInst()->GetContext()->GSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	Device::GetInst()->GetContext()->PSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}
