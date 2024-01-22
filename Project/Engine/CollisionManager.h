#pragma once
#include "Layer.h"

union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};

class Collider2D;

class CollisionManager
{
	SINGLETON(CollisionManager)
private:
	UINT m_CollisionMatrix[(UINT)LAYER_TYPE::END];
	std::map<UINT_PTR, bool> m_prevInfo;

public:
	void Init();
	void Update();

public:
	void Clear();

	void CollisionMatrixCheck(LAYER_TYPE _layerType1, LAYER_TYPE _layerType2, bool _check = true);

	void LayerCollisionCheck(UINT _left, UINT _right);
	bool ColliderCollisionCheck(Collider2D* _left, Collider2D* _right);

	bool BoxToBox(Collider2D* _left, Collider2D* _right);
	bool CircleToCircle(Collider2D* _left, Collider2D* _right);
	bool BoxToCircle(Collider2D* _box, Collider2D* _circle);

};

