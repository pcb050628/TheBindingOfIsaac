#include "pch.h"
#include "CollisionManager.h"

#include "ChapterManager.h"
#include "Chapter.h"
#include "Room.h"
#include "Layer.h"
#include "Actor.h"

#include "Collider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

CollisionManager::CollisionManager()
	: m_CollisionMatrix()
{

}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::Update()
{
	for (int row = 0; row < (int)LayerType::END; row++)
	{
		for (int col = row; col < (int)LayerType::END; col++)
		{
			if (m_CollisionMatrix[row][col])
			{
				LayerCheck(row, col);
			}
		}
	}
}

void CollisionManager::LateUpdate()
{
}

void CollisionManager::LayerCheck(int _layer1, int _layer2)
{
	vector<Collider*> l1Col = ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetLayer((LayerType)_layer1).GetAllCollider();
	vector<Collider*> l2Col = ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetLayer((LayerType)_layer2).GetAllCollider();

	for (int one = 0; one < l1Col.size(); one++)
	{
		for (int two = 0; two < l2Col.size(); two++)
		{
			COLLIDER_ID ID(l1Col[one]->GetID(), l2Col[two]->GetID());

			map<COLLIDER_ID, bool>::iterator iter = m_mapID.find(ID);

			if (iter == m_mapID.end())
			{
				m_mapID.insert(make_pair(ID, false));
				iter = m_mapID.find(ID);
			}

			if (isCollision(l1Col[one], l2Col[two]))
			{
				if (iter->second == false)
				{
					l1Col[one]->BeginOverlap(l1Col[one], l2Col[two]->GetOwner(), l2Col[two]);
					l2Col[two]->BeginOverlap(l2Col[two], l1Col[one]->GetOwner(), l1Col[one]);
				}
				else
				{
					l1Col[one]->Overlap(l1Col[one], l2Col[two]->GetOwner(), l2Col[two]);
					l2Col[two]->Overlap(l2Col[two], l1Col[one]->GetOwner(), l1Col[one]);
				}

				iter->second = true;
			}
			else
			{
				if (iter->second == true)
				{
					l1Col[one]->EndOverlap(l1Col[one], l2Col[two]->GetOwner(), l2Col[two]);
					l2Col[two]->EndOverlap(l2Col[two], l1Col[one]->GetOwner(), l1Col[one]);
				}

				iter->second = false;
			}
		}
	}
}

bool CollisionManager::isCollision(Collider* col1, Collider* col2)
{
	if (!IsValid(col1->GetOwner()) || !IsValid(col2->GetOwner()))
		return false;

	if (col1->GetColliderType() == col2->GetColliderType())
		if (col1->GetColliderType() == ColliderType::Box)
			return isCollision(static_cast<BoxCollider*>(col1), static_cast<BoxCollider*>(col2));
		else
			return isCollision(static_cast<CircleCollider*>(col1), static_cast<CircleCollider*>(col2));
	else if (col1->GetColliderType() == ColliderType::Box)
		return isCollision(static_cast<BoxCollider*>(col1), static_cast<CircleCollider*>(col2));
	else
		return isCollision(static_cast<BoxCollider*>(col2), static_cast<CircleCollider*>(col1));
}

bool CollisionManager::isCollision(BoxCollider* col1, CircleCollider* col2)
{
	if (fabs((col1->GetFinalPos() - col2->GetFinalPos()).Length()) < fabs(col1->GetScale().Length() + col2->GetRadius()))
		return true;

	return false;
}

bool CollisionManager::isCollision(BoxCollider* col1, BoxCollider* col2)
{
	if (col1->GetFinalPos().x - col1->GetScale().x > col2->GetFinalPos().x + col2->GetScale().x
		|| col1->GetFinalPos().x + col1->GetScale().x < col2->GetFinalPos().x - col2->GetScale().x)
		return true;

	if (col1->GetFinalPos().y - col1->GetScale().y < col2->GetFinalPos().y + col2->GetScale().y
		|| col1->GetFinalPos().y + col1->GetScale().y > col2->GetFinalPos().y - col2->GetScale().y)
		return true;

	return false;
}

bool CollisionManager::isCollision(CircleCollider* col1, CircleCollider* col2)
{
	if (fabs((col1->GetFinalPos() - col2->GetFinalPos()).Length()) < fabs(col1->GetRadius() + col2->GetRadius()))
		return true;

	return false;
}

