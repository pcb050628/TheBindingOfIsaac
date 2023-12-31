#include "pch.h"
#include "Layer.h"
#include "GameObject.h"
#include "GarbageCollection.h"

Layer::Layer()
	: m_Parents()
	, m_Gobjs()
	, m_iLayerIdx(-1)
{
}

Layer::~Layer()
{
	auto iter = m_Parents.begin();
	for (; iter != m_Parents.end(); )
	{
		delete (*iter);
		iter = m_Parents.erase(iter);
	}
}

void Layer::Update()
{
	for (GameObject* gobj : m_Parents)
	{
		gobj->Update();
	}
}

void Layer::LateUpdate()
{
	for (GameObject* gobj : m_Parents)
	{
		gobj->LateUpdate();
	}
}

void Layer::Render()
{
	for (GameObject* gobj : m_Gobjs)
	{
		gobj->Render();
	}

	auto iter = m_Parents.begin();
	for (; iter != m_Parents.end();)
	{
		if (!IsValid(*iter))
		{
			GarbageCollection::GetInst()->Add(*iter);
			iter = m_Parents.erase(iter);
		}
		else
			iter++;
	}
}

void Layer::Clear()
{
	auto iter = m_Gobjs.begin();
	for (; iter != m_Gobjs.end();)
	{
		iter = m_Gobjs.erase(iter);
	}
}

void Layer::AddObject(GameObject* _obj, bool _bMove)
{
	if (!_obj->GetParent())
	{
		if (_obj->m_iLayerIdx != -1)
		{
			_obj->DisconnectWithLayer();
		}
	}

	m_Parents.push_back(_obj);

	std::queue<GameObject*> queueObj;
	queueObj.push(_obj);

	while (!queueObj.empty())
	{
		GameObject* front = queueObj.front();

		for (int i = 0; i < front->m_ChildObjs.size(); i++)
		{
			queueObj.push(front->m_ChildObjs[i]);
		}

		if (front == _obj)
			front->m_iLayerIdx = m_iLayerIdx;
		else
		{
			if (_bMove)
				front->m_iLayerIdx = m_iLayerIdx;
			else if(front->m_iLayerIdx == -1)
				front->m_iLayerIdx = m_iLayerIdx;
		}

		queueObj.pop();
	}
}

void Layer::DetachGameObject(GameObject* _obj)
{
	assert(_obj->m_iLayerIdx = m_iLayerIdx);

	if (_obj->GetParent())
	{
		_obj->m_iLayerIdx = -1;
	}
	else
	{
		auto iter = m_Parents.begin();
		for (; iter != m_Parents.end(); iter++)
		{
			if (*iter == _obj)
			{
				m_Parents.erase(iter);
				return;
			}
		}

		assert(nullptr);
	}
}

void Layer::RegisterObject(GameObject* _obj)
{
	m_Gobjs.push_back(_obj);
}
