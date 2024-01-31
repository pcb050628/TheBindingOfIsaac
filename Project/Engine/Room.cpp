#include "pch.h"
#include "Room.h"

#include "GameObject.h"
#include "Transform.h"
#include "Layer.h"

Room::Room() : Resource(RESOURCE_TYPE::ROOM)
	, m_Layers()
	, m_RoomType(ROOM_TYPE::Common)
	, Left(nullptr)
	, Right(nullptr)
	, Top(nullptr)
	, Bottom(nullptr)
	, m_bEditMode(false)
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

bool Room::Load(const std::wstring& _strFileName)
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + _strFileName;
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(filePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	SetName(szName);

	if (fileStream.is_open())
	{
		std::wstring line;

		GameObject* gobj = nullptr;
		while (true)
		{
			std::getline(fileStream, line);

			if (line == L"END")
				break;

			//��ġ �̸� ������ �ۼ��Ǿ� ����

			//Ÿ�� ��ġ ���
			std::wstring xstr = line.substr(0, 1);
			std::wstring ystr = line.substr(2, 1);
			int x = atoi(std::string(xstr.begin(), xstr.end()).c_str());
			int y = atoi(std::string(ystr.begin(), ystr.end()).c_str());
			Vec2 tile(x, y);

			//���ϸ� ���
			std::getline(fileStream, line);
			std::wstring objName = line + L".txt";

			//���̾� ���
			std::getline(fileStream, line);
			LAYER_TYPE layer = (LAYER_TYPE)atoi(std::string(line.begin(), line.end()).c_str());

			//�߰�
			gobj = new GameObject;
			gobj->Load(objName);
			gobj->GetTransform()->SetRelativePos(GetPosByTile(tile));
			AddGameObject(gobj, layer);
		}

		fileStream.close();
		return true;
	}
	else
		return false;
}

bool Room::Save()
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(m_Type) + GetName();
	filePath += L".txt";
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		//��ġ ���ͼ� Ÿ����ġ�� ���� �� Ÿ����ġ ����
		//������Ʈ ����, ������Ʈ ������ �̸����� ���ϸ��� ���� �̸��� ���͵� �ɵ�

		for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
		{
			std::vector<GameObject*> obj = m_Layers[i]->GetParentObjects();
			for (int j = 0; j < obj.size(); j++)
			{
				Vec2 tile = GetTileByPos(obj[j]->GetTransform()->GetRelativePos());
				fileStream <<  (int)tile.x << L"|" << (int)tile.y  << std::endl;
				fileStream << obj[j]->GetName() << std::endl;
				fileStream << obj[j]->GetLayerIdx() << std::endl;
			}
		}

		fileStream << L"END";

		fileStream.close();

		return true;
	}
	else
		return false;

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
		//���콺 Ŭ������ ��ǥ �޾ƿ���
		//GetTileByPos �Լ��� Ÿ�Ϲ޾ƿ���
		//GetPosByTile �Լ��� ��Ȯ�� Ÿ�� ��ġ �޾ƿ���
		//Ÿ�� �߰� ��ġ�� ������Ʈ ����
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


Vec3 Room::GetPosByTile(Vec2 _tile)
{
	float x = -725 + (100 * _tile.y);
	float y = 425 - (100 * _tile.x);
	return Vec3(x, y, -y);
}

Vec2 Room::GetTileByPos(Vec3 _pos)
{
	return Vec2((_pos.y - 725) / 100, (_pos.x - 425) / 100);
}

void Room::AddObject(GameObject* _obj, LAYER_TYPE _layr, bool _bMove)
{
	m_Layers[(UINT)_layr]->AddObject(_obj, _bMove);
	_obj->m_RoomNumber = m_RoomNumber;
}

void Room::AddObjectByTile(GameObject* _obj, LAYER_TYPE _layr, Vec2 _tilePos, bool _bMove)
{
	m_Layers[(UINT)_layr]->AddObject(_obj, _bMove);
	_obj->GetTransform()->SetRelativePos(Vec3(GetPosByTile(_tilePos).x, GetPosByTile(_tilePos).y, 0.f));
	_obj->m_RoomNumber = m_RoomNumber;
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

			// ���̾ �ԷµǴ� ������Ʈ ����, �� �ؿ� �޸� �ڽĵ���� ��� Ȯ��
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

void Room::GetAllObject(std::vector<GameObject*>& _out)
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		if (0 < m_Layers[i]->m_Parents.size())
		{
			_out.insert(_out.end(), m_Layers[i]->m_Parents.begin(), m_Layers[i]->m_Parents.end());
		}
	}
}
