#pragma once
#include "define.h"
#include "pch.h"
#include "Layer.h"

class Collider;

struct COLLIDER_ID
{
	INT_PTR left;
	INT_PTR right;

	bool operator == (const COLLIDER_ID& _other) const
	{
		if (left == _other.left && right == _other.right)
			return true;
		return false;
	}

	bool operator < (const COLLIDER_ID& _other) const
	{
		if (left < _other.left)
			return true;
		else if (left > _other.left)
			return false;
		else
		{
			if (right < _other.right)
				return true;
			else
				return false;
		}
	}

	COLLIDER_ID()
		: left(0)
		, right(0)
	{}

	COLLIDER_ID(INT_PTR _Left, INT_PTR _Right)
		: left(_Left)
		, right(_Right)
	{}
};

class CollisionManager
{
	SINGLETON(CollisionManager)
private:
	bool						m_CollisionMatrix[(int)LayerType::END][(int)LayerType::END];
	std::map<COLLIDER_ID, bool>	m_mapID;

public:
	void Update();
	void LateUpdate();

	void LayerCollisionCheck(LayerType _layer1, LayerType _layer2, bool value)
	{
		int ly1 = (int)_layer1;
		int ly2 = (int)_layer2;

		if (ly2 < ly1)
		{
			int tmp = ly1;
			ly1 = ly2;
			ly2 = tmp;
		}

		m_CollisionMatrix[ly1][ly2] = value;
	}

	void ResetCollisionMatrix()
	{
		for (int row = 0; row < (int)LayerType::END; row++)
		{
			for (int col = row; col < (int)LayerType::END; col++)
			{
				m_CollisionMatrix[row][col] = false;
			}
		}
	}

	void LayerCheck(int _layer1, int _layer2);
	bool isCollision(Collider* col1, Collider* col2);
	bool isCollision(class BoxCollider* col1, class CircleCollider* col2);
	bool isCollision(class BoxCollider* col1, class BoxCollider* col2);
	bool isCollision(class CircleCollider* col1, class CircleCollider* col2);
};

