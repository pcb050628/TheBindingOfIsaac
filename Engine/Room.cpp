#include "pch.h"
#include "Room.h"

#include "GameObject.h"
#include "Layer.h"

Room::Room() : Resource(RESOURCE_TYPE::ROOM)
	, m_Layers()
	, m_Type(RoomType::Common)
	, Left(nullptr)
	, Right(nullptr)
	, Top(nullptr)
	, Bottom(nullptr)
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		m_Layers[i] = new Layer;
		m_Layers[i]->m_iLayerIdx = i;
	}
}

Room::~Room()
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		delete m_Layers[i];
	}
}

bool Room::Load(const std::wstring& _strFilePath)
{
	return false;
}

bool Room::Save()
{
	return false;
}

void Room::Clear()
{
	for (Layer* layer : m_Layers)
	{
		layer->Clear();
	}
}

void Room::Enter()
{
}

void Room::Update()
{
	for (Layer* layer : m_Layers)
	{
		layer->Update();
	}

	if (m_bEditMode)
	{
		//마우스 클릭으로 좌표 받아오고
		//GetTileByPos 함수로 타일받아오고
		//GetPosByTile 함수로 정확한 타일 중간위치 받아오고
		//타일 중간 위치에 오브젝트 생성
	}
}

void Room::LateUpdate()
{
	for (Layer* layer : m_Layers)
	{
		layer->LateUpdate();
	}
}

void Room::Exit()
{
}


Vec2 Room::GetPosByTile(Vec2 _tile)
{
	return Vec2(-725 + (100 * _tile.y), 425 - (100 * _tile.x));
}

Vec2 Room::GetTileByPos(Vec2 _pos)
{
	return Vec2((_pos.y - 725) / 100, (_pos.x - 425) / 100);
}

void Room::AddObjectByTile(GameObject* _obj, LAYER_TYPE _layr, Vec2 _tilePos, bool _bMove)
{
	m_Layers[(UINT)_layr]->AddObject(_obj, _bMove);
	_obj->GetTransform()->SetRelativePos(Vec3(GetPosByTile(_tilePos).x, GetPosByTile(_tilePos).y, 0.f));
}

void Room::DetachGameObject(GameObject* _obj)
{
	m_Layers[_obj->m_iLayerIdx]->DetachGameObject(_obj);
}

GameObject* Room::FindObject(const std::wstring& _strName)
{
	for (UINT i = 0; i < (UINT)LAYER_TYPE::END; ++i)
	{
		const vector<GameObject*>& vecParent = m_Layers[i]->GetParentObjects();

		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			list<GameObject*> queue;
			queue.push_back(vecParent[j]);

			// 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
			while (!queue.empty())
			{
				GameObject* pObject = queue.front();
				queue.pop_front();

				const vector<GameObject*>& vecChild = pObject->GetChild();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					queue.push_back(vecChild[k]);
				}

				if (_strName == pObject->GetName())
				{
					return pObject;
				}
			}
		}
	}

	return nullptr;
}
