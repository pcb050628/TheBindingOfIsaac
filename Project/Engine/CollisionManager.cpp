#include "pch.h"
#include "CollisionManager.h"

#include "ChapterManager.h"
#include "Chapter.h"
#include "Room.h"

#include "GameObject.h"
#include "Collider2D.h"

CollisionManager::CollisionManager()
	: m_CollisionMatrix{}
	, m_prevInfo{}
{}

CollisionManager::~CollisionManager()
{}

void CollisionManager::Init()
{
}

void CollisionManager::Update()
{
	for (UINT row = 0; row < (UINT)LAYER_TYPE::END; row++)
	{
		for (UINT col = row; col < (UINT)LAYER_TYPE::END; col++)
		{
			if (!(m_CollisionMatrix[row] & (1 << col)))
				continue;

			LayerCollisionCheck(row, col);
		}
	}
}

void CollisionManager::Clear()
{
}

void CollisionManager::CollisionMatrixCheck(LAYER_TYPE _layerType1, LAYER_TYPE _layerType2, bool _check)
{
	UINT row = (UINT)_layerType1;
	UINT col = (UINT)_layerType2;

	if (row > col)
	{
		UINT tmp = row;
		row = col;
		col = row;
	}

	if(_check)
		m_CollisionMatrix[row] |= (1 << col);
	else
		m_CollisionMatrix[row] &= ~(1 << col);
}

void CollisionManager::LayerCollisionCheck(UINT _left, UINT _right)
{
	Layer* layerLeft = ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetLayer(_left);
	Layer* layerRight = ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetLayer(_right);

	const std::vector<GameObject*>& objLeft = layerLeft->GetGameObject();
	const std::vector<GameObject*>& objRight = layerRight->GetGameObject();

	for (int i = 0; i < objLeft.size(); i++)
	{
		for (int j = i; j < objRight.size(); i++)
		{
			if (objLeft[i] == objRight[j] || !(objLeft[i]->GetCollider2D() && objRight[j]->GetCollider2D()))
				continue;

			CollisionID id = {};
			id.LeftID = objLeft[i]->GetCollider2D()->GetID();
			id.RightID = objRight[j]->GetCollider2D()->GetID();

			auto iter = m_prevInfo.begin();
			if (m_prevInfo.find(id.id) == m_prevInfo.end())
			{
				m_prevInfo.insert(make_pair(id.id, false));
				iter = m_prevInfo.find(id.id);
			}

			bool dead = objLeft[i]->GetIsDead() || objRight[j]->GetIsDead();

			if (ColliderCollisionCheck(objLeft[i]->GetCollider2D(), objRight[j]->GetCollider2D()))
			{
				if (!dead)
				{
					if (iter->second)
					{
						objLeft[i]->GetCollider2D()->Overlap(objRight[j]->GetCollider2D());
						objRight[j]->GetCollider2D()->Overlap(objLeft[i]->GetCollider2D());
					}
					else
					{
						objLeft[i]->GetCollider2D()->BeginOverlap(objRight[j]->GetCollider2D());
						objRight[j]->GetCollider2D()->BeginOverlap(objLeft[i]->GetCollider2D());
						iter->second = true;
					}
				}
				else
				{
					objLeft[i]->GetCollider2D()->EndOverlap(objRight[j]->GetCollider2D());
					objRight[j]->GetCollider2D()->EndOverlap(objLeft[i]->GetCollider2D());
					iter->second = false;
				}
			}
			else
			{
				if (iter->second)
				{
					objLeft[i]->GetCollider2D()->EndOverlap(objRight[j]->GetCollider2D());
					objRight[j]->GetCollider2D()->EndOverlap(objLeft[i]->GetCollider2D());
					iter->second = false;
				}
			}
		}
	}
}

bool CollisionManager::ColliderCollisionCheck(Collider2D* _left, Collider2D* _right)
{
	if (_left->GetCollider2DType() == COLLIDER2D_TYPE::BOX &&
		_left->GetCollider2DType() == COLLIDER2D_TYPE::BOX)
	{
		return BoxToBox(_left, _right);
	}
	else if (_left->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE &&
		_left->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE)
	{
		return CircleToCircle(_left, _right);
	}
	else
	{
		return BoxToCircle(_left, _right);
	}
}

bool CollisionManager::BoxToBox(Collider2D* _left, Collider2D* _right)
{
	const Matrix& matLeft = _left->GetColliderWorldMat();
	const Matrix& matRight = _right->GetColliderWorldMat();

	// Rect Local
	// 0 -- 1
	// |    |
	// 3 -- 2
	static Vec3 arrRect[4] = { Vec3(-0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, -0.5f, 0.f)
							  , Vec3(-0.5f, -0.5f, 0.f) };

	Vec3 arrProj[4] = {};

	Vec3 tmp1 = DirectX::XMVector3TransformCoord(arrRect[1], matLeft);
	Vec3 tmp2 = DirectX::XMVector3TransformCoord(arrRect[0], matLeft);
	arrProj[0] = tmp1 - tmp2;

	tmp1 = DirectX::XMVector3TransformCoord(arrRect[3], matLeft);
	tmp2 = DirectX::XMVector3TransformCoord(arrRect[0], matLeft);
	arrProj[1] = tmp1 - tmp2;

	tmp1 = DirectX::XMVector3TransformCoord(arrRect[1], matRight);
	tmp2 = DirectX::XMVector3TransformCoord(arrRect[0], matRight);
	arrProj[2] = tmp1 - tmp2;

	tmp1 = DirectX::XMVector3TransformCoord(arrRect[3], matRight);
	tmp2 = DirectX::XMVector3TransformCoord(arrRect[0], matRight);
	arrProj[3] = tmp1 - tmp2;

	tmp1 = DirectX::XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matRight);
	tmp2 = DirectX::XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matLeft);
	Vec3 vCenter = tmp1 - tmp2;

	// i ��° ���������� 4���� ǥ�麤�͸� ������Ų��.
	for (int i = 0; i < 4; ++i)
	{
		// i ��° ǥ����͸� ���������� ��´�
		Vec3 vProj = arrProj[i];

		// �������ͷ� ���� ������ ��� ������ ���̸� ���� �� �ְ� �Ѵ�.
		vProj.Normalize();

		// ������ ���̸� ������ų ����
		float ProjAcc = 0.f;

		// �ݺ��� ���鼭 4���� ǥ�麤�͸� ������ ���������� �������Ѽ� ���̸� �����޴´�.
		for (int j = 0; j < 4; ++j)
		{
			ProjAcc += fabs(vProj.Dot(arrProj[j]));
		}

		// ������ ������ ���ݾ� ��ģ ���̰� �ʿ��ϱ� ������ ��ü ��ģ���̸� 2 �� ������
		ProjAcc /= 2.f;

		// �� �浹ü�� �߽��� ���� ���͵� ������Ų��.
		float fCenterDist = fabs(vProj.Dot(vCenter));

		// �߽��� ���� ���͸� ������Ų ���̰�, ǥ���� ������Ų ������ ���ݺ��� ũ�ٸ� 
		// ���� �и���ų �� �ִ�.
		if (ProjAcc < fCenterDist)
		{
			return false;
		}
	}

	// 4���� �׽�Ʈ���� �и��� �� ������.
	return true;
}

bool CollisionManager::CircleToCircle(Collider2D* _left, Collider2D* _right)
{
	Vec3 leftPos = _left->GetColliderWorldMat().Translation();
	Vec3 rightPos = _right->GetColliderWorldMat().Translation();

	float dist = (leftPos - rightPos).Length();

	if (fabs(dist) < fabs(_left->GetOffsetScale().x + _right->GetOffsetScale().x))
		return true;
	else
		return false;
}

bool CollisionManager::BoxToCircle(Collider2D* _box, Collider2D* _circle)
{
	if (_box->GetCollider2DType() != COLLIDER2D_TYPE::BOX)
	{
		auto tmp = _box;
		_box = _circle;
		_circle = tmp;
	}

	const Matrix& matBox = _box->GetColliderWorldMat();
	const Matrix& matCircle = _circle->GetColliderWorldMat();

	// Rect Local
	// 0 -- 1
	// |    |
	// 3 -- 2
	static Vec3 arrRect[4] = { Vec3(-0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, 0.5f, 0.f)
							  , Vec3(0.5f, -0.5f, 0.f)
							  , Vec3(-0.5f, -0.5f, 0.f) };

	Vec3 arrEdge[4] = {};

	arrEdge[0] = DirectX::XMVector3TransformCoord(arrRect[0], matBox);
	arrEdge[1] = DirectX::XMVector3TransformCoord(arrRect[1], matBox);
	arrEdge[2] = DirectX::XMVector3TransformCoord(arrRect[2], matBox);
	arrEdge[3] = DirectX::XMVector3TransformCoord(arrRect[3], matBox);

	Vec3 circlePos = DirectX::XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCircle);
	Vec3 boxPos = DirectX::XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matBox);

	Vec3 vProj = boxPos - circlePos;

	float dist = vProj.Length();
	Vec3 vLine = {};

	int idx = 0;
	for (int i = 0; i < 4; i++)
	{
		float mostShort = fabs((circlePos - arrEdge[idx]).Length());
		float dist = fabs((circlePos - arrEdge[i]).Length());
		if (mostShort > dist)
			idx = i;
	}

	if (idx == 0)
		vLine = arrEdge[0] - arrEdge[1];
	else if(idx == 1)
		vLine = arrEdge[1] - arrEdge[0];
	else if (idx == 2)
		vLine = arrEdge[2] - arrEdge[3];
	else if (idx == 3)
		vLine = arrEdge[3] - arrEdge[2];

	if (fabs(dist) < vProj.Dot(vLine))
		return false;
	else
		return true;
}
