#include "pch.h"
#include "StructuredBuffer.h"

#include "Device.h"

StructuredBuffer::StructuredBuffer()
	: m_SB(nullptr)
	, m_SRV(nullptr)
	, m_SB_Read(nullptr)
	, m_SB_Write(nullptr)
	, m_ElementCount(0)
	, m_ElementSize(0)
	, m_SBType(SB_TYPE::READ_ONLY)
	, m_bSysMemMove(false)
{
}

StructuredBuffer::StructuredBuffer(const StructuredBuffer& _origin)
	: Entity(_origin)
	, m_RecentNum_SRV(-1)
	, m_RecentNum_UAV(-1)

{
	Create(_origin.m_ElementSize, _origin.m_ElementCount, _origin.m_SBType, _origin.m_bSysMemMove);
}

StructuredBuffer::~StructuredBuffer()
{
}

int StructuredBuffer::Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type, bool _bSysMemMove, void* _pSysMem)
{
	m_SB = nullptr;
	m_SRV = nullptr;
	m_SB_Read = nullptr;
	m_SB_Write = nullptr;

	HRESULT hr = S_OK;

	m_bSysMemMove = _bSysMemMove;
	m_SBType = _type;
	m_ElementSize = _elementSize;
	m_ElementCount = _elementCount;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = m_ElementSize * m_ElementCount;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = m_ElementSize;
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	if (nullptr == _pSysMem)
	{
		hr = Device::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, m_SB.GetAddressOf());
		if (FAILED(hr)) return E_FAIL;
	}
	else
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pSysMem;
		hr = Device::GetInst()->GetDevice()->CreateBuffer(&desc, &tSub, m_SB.GetAddressOf());
		if (FAILED(hr)) return E_FAIL;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = m_ElementCount;

	hr = Device::GetInst()->GetDevice()->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	if (SB_TYPE::READ_WRITE == m_SBType)
	{
		desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_ElementCount;

		hr = Device::GetInst()->GetDevice()->CreateUnorderedAccessView(m_UA.Get(), &UAVDesc, m_UAV.GetAddressOf());
		if (FAILED(hr)) return E_FAIL;
	}

	if (m_bSysMemMove)
	{
		// 쓰기 버퍼
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = Device::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, m_SB_Write.GetAddressOf());
		if (FAILED(hr)) return E_FAIL;

		// 읽기 버퍼
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		hr = Device::GetInst()->GetDevice()->CreateBuffer(&desc, nullptr, m_SB_Read.GetAddressOf());
		if (FAILED(hr)) return E_FAIL;
	}

	return S_OK;
}

void StructuredBuffer::SetData(void* _data, UINT _elementCount)
{
	if (_data == nullptr)
		return;

	if (!m_bSysMemMove)
	{
		MessageBoxW(nullptr, L"데이터를 입력할 수 없는 버퍼입니다.", L"structured buffer Error", MB_OK);
		return;
	}

	if (_elementCount == 0)
		_elementCount = m_ElementCount;

	if (_elementCount > m_ElementCount)
		Create(m_ElementSize, _elementCount, m_SBType, m_bSysMemMove);

	D3D11_MAPPED_SUBRESOURCE data = {};
	Device::GetInst()->GetContext()->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, _data, m_ElementSize * _elementCount);
	Device::GetInst()->GetContext()->Unmap(m_SB_Write.Get(), 0);

	Device::GetInst()->GetContext()->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

void StructuredBuffer::GetData(void* _destData, UINT _elementCount)
{
	if (!m_bSysMemMove)
	{
		MessageBoxW(nullptr, L"데이터를 읽어들 일 수 없는 버퍼입니다.", L"structured buffer Error", MB_OK);
		return;
	}

	if (_elementCount == 0)
		_elementCount = m_ElementCount;

	Device::GetInst()->GetContext()->CopyResource(m_SB_Read.Get(), m_SB.Get());

	D3D11_MAPPED_SUBRESOURCE data = {};
	Device::GetInst()->GetContext()->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &data);
	memcpy(_destData, data.pData, m_ElementSize * _elementCount);
	Device::GetInst()->GetContext()->Unmap(m_SB_Read.Get(), 0);
}

void StructuredBuffer::UpdateData(int _regiNum)
{
	Device::GetInst()->GetContext()->VSSetShaderResources(_regiNum, 1, m_SRV.GetAddressOf());
	Device::GetInst()->GetContext()->HSSetShaderResources(_regiNum, 1, m_SRV.GetAddressOf());
	Device::GetInst()->GetContext()->DSSetShaderResources(_regiNum, 1, m_SRV.GetAddressOf());
	Device::GetInst()->GetContext()->GSSetShaderResources(_regiNum, 1, m_SRV.GetAddressOf());
	Device::GetInst()->GetContext()->PSSetShaderResources(_regiNum, 1, m_SRV.GetAddressOf());
}

int StructuredBuffer::UpdateData_CS_SRV(int _regiNum)
{
	if (nullptr == m_SRV)
		return E_FAIL;

	m_RecentNum_SRV = _regiNum;

	Device::GetInst()->GetContext()->CSSetShaderResources(_regiNum, 1, m_SRV.GetAddressOf());
	return S_OK;
}

int StructuredBuffer::UpdateData_CS_UAV(int _regiNum)
{
	if (nullptr == m_SRV)
		return E_FAIL;

	m_RecentNum_UAV = _regiNum;

	UINT i = -1;
	Device::GetInst()->GetContext()->CSSetUnorderedAccessViews(_regiNum, 1, m_UAV.GetAddressOf(), &i);
	return S_OK;
}

void StructuredBuffer::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* srv = nullptr;

	Device::GetInst()->GetContext()->CSSetShaderResources(m_RecentNum_SRV, 1, &srv);
}

void StructuredBuffer::Clear_CS_UAV()
{
	ID3D11UnorderedAccessView* uav = nullptr;

	UINT i = -1;
	Device::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_RecentNum_UAV, 1, &uav, &i);
}
