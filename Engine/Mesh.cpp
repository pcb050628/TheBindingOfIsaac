#include "pch.h"
#include "Mesh.h"

#include "RenderManager.h"

Mesh::Mesh() : Resource(ResourceType::MESH)
	, m_pVtxBuffer(nullptr)
	, m_pIdxBuffer(nullptr)
	, m_VBDesc()
	, m_IBDesc()
	, m_iVtxCount()
	, m_iIdxCount()
	, m_pVtxSysMem(nullptr)
	, m_pIdxSysMem(nullptr)
{
}

Mesh::~Mesh()
{
}

int Mesh::Create(void* _vtx, UINT _vtxCount, void* _idx, UINT _idxCount)
{
	{
		m_VBDesc = {};
		m_VBDesc.ByteWidth = sizeof(Vtx) * _vtxCount;
		m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_VBDesc.StructureByteStride = sizeof(Vtx);

		m_VBDesc.CPUAccessFlags = 0;
		m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA data = { };
		data.pSysMem = _vtx;

		if (FAILED(RenderManager::GetInst()->GetDevice()->CreateBuffer(&m_VBDesc, &data, m_pVtxBuffer.GetAddressOf())))
		{
			MessageBox(nullptr, "Failed Vertex Buffer Create", "Mesh Create Fail", MB_OK);
			return E_FAIL;
		}
	}

	{
		m_IBDesc = {};
		m_IBDesc.ByteWidth = sizeof(UINT) * _idxCount;
		m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_IBDesc.StructureByteStride = sizeof(UINT);

		m_IBDesc.CPUAccessFlags = 0;
		m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA data = { };
		data.pSysMem = _vtx;

		if (FAILED(RenderManager::GetInst()->GetDevice()->CreateBuffer(&m_IBDesc, &data, m_pIdxBuffer.GetAddressOf())))
		{
			MessageBox(nullptr, "Failed Index Buffer Create", "Mesh Create Fail", MB_OK);
			return E_FAIL;
		}
	}

	m_pVtxSysMem = new Vtx[_vtxCount];
	m_pIdxSysMem = new UINT[_idxCount];

	memcpy(m_pVtxSysMem, _vtx, sizeof(Vtx) * _vtxCount);
	memcpy(m_pIdxSysMem, _idx, sizeof(UINT) * _idxCount);

	return S_OK;
}

void Mesh::UpdateData()
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	RenderManager::GetInst()->GetContext()->IASetVertexBuffers(0, m_iVtxCount, m_pVtxBuffer.GetAddressOf(), &iStride, &iOffset);
	RenderManager::GetInst()->GetContext()->IASetIndexBuffer(m_pIdxBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::Render()
{
	UpdateData();
	RenderManager::GetInst()->GetContext()->DrawIndexed(m_iIdxCount, 0, 0);
}
