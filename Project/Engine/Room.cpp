#include "pch.h"
#include "Room.h"

#include "GameObject.h"
#include "Transform.h"
#include "Layer.h"

Room::Room() : Resource(RESOURCE_TYPE::ROOM)
	, m_Info{ NULL, ROOM_TYPE::Common, false, {}, {} }
	, m_Layers()
	, m_MainCam(nullptr)
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

bool Room::Load(const std::wstring& _strFileName, bool _isFullPath)
{
	filesystem::path filePath;
	if (_isFullPath)
		filePath = _strFileName;
	else
		filePath = GetContentPath() + GetResourceFolderPath(m_Type) + _strFileName;
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

			if (line == L"[RoomType]")
			{
				std::getline(fileStream, line);
				m_Info.RoomType = (ROOM_TYPE)atoi(ToString(line).c_str());
			}
			else if (line == L"[ChapterLevel]")
			{
				std::getline(fileStream, line);
				m_Info.ChapterLevel = (UINT)atoi(ToString(line).c_str());
			}
			else
			{
				//위치 -> 이름 순으로 작성되어 있음

				//타일 위치 얻기
				std::wstring xstr = line.substr(0, 1);
				std::wstring ystr = line.substr(2, 1);
				int x = atoi(std::string(xstr.begin(), xstr.end()).c_str());
				int y = atoi(std::string(ystr.begin(), ystr.end()).c_str());
				Vec2 tile((float)x, (float)y);

				//파일명 얻기
				std::getline(fileStream, line);
				std::wstring objName = line + L".txt";

				//레이어 얻기
				std::getline(fileStream, line);
				LAYER_TYPE layer = (LAYER_TYPE)atoi(std::string(line.begin(), line.end()).c_str());

				m_Info.TileObjectInfo[x][y] = objName;
				m_Info.TileLayerInfo[x][y] = layer;
			}
		}

		fileStream.close();
		return true;
	}
	else
		return false;
}

bool Room::Save()
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(m_Type) + GetResourceName();
	filePath += L".room";
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		fileStream << L"[RoomType]\n" << (UINT)m_Info.RoomType << std::endl;
		fileStream << L"[ChapterLevel]\n" << m_Info.ChapterLevel << std::endl;

		for (int row = 0; row < 9; row++)
		{
			for (int col = 0; col < 15; col++)
			{
				fileStream << row << L"|" << col << std::endl;
				fileStream << m_Info.TileObjectInfo[row][col] << std::endl;
				fileStream << (UINT)m_Info.TileLayerInfo[row][col] << std::endl;
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
	if (!m_Info.IsCompleted)
	{
		GameObject* gobj = nullptr;

		for (int row = 0; row < 9; row++)
		{
			for (int col = 0; col < 15; col++)
			{
				const std::wstring& objName = m_Info.TileObjectInfo[row][col];
				LAYER_TYPE layer = m_Info.TileLayerInfo[row][col];

				if (objName == L"")
					continue;

				gobj = new GameObject;
				gobj->Load(objName);
				gobj->GetTransform()->SetRelativePos(GetPosByTile(row, col));
				AddGameObject(gobj, layer);
			}
		}
	}

	for (Layer* layer : m_Layers)
	{
		layer->Enter();
	}
}

void Room::Update()
{
	for (Layer* layer : m_Layers)
	{
		layer->Update();
	}
}

void Room::LateUpdate()
{
	for (Layer* layer : m_Layers)
	{
		layer->LateUpdate();
	}

	//Room 클리어 체크
	{
		std::vector<GameObject*> monsters = GetLayer(LAYER_TYPE::Monster)->GetParentObjects();
		if (monsters.empty())
			m_Info.IsCompleted = true;
		else
		{
			UINT check = 0;
			for (GameObject* monster : monsters)
			{
				if (!IsValid(monster))
				{
					check++;
				}
			}

			if (check == (UINT)monsters.size())
				m_Info.IsCompleted = true;
		}
	}
}

void Room::Exit()
{
	for (Layer* layer : m_Layers)
	{
		layer->Exit();
	}

	for (Layer* layer : m_Layers)
	{
		if (m_Info.IsCompleted && layer->m_iLayerIdx == (UINT)LAYER_TYPE::Camera)
			continue;

		layer->DeleteAll();
	}
}


Vec3 Room::GetPosByTile(UINT _x, UINT _y)
{
	float x = (float)(-725 + (100 * _y));
	float y = (float)(425 - (100 * _x));

	return Vec3(x, y, -y);
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
	_obj->m_RoomNumber = m_Info.RoomNumber;

	if (!m_MainCam && LAYER_TYPE::Camera == _layr)
		m_MainCam = _obj;
}

void Room::AddObjectByTile(GameObject* _obj, LAYER_TYPE _layr, Vec2 _tilePos, bool _bMove)
{
	m_Layers[(UINT)_layr]->AddObject(_obj, _bMove);
	_obj->GetTransform()->SetRelativePos(Vec3(GetPosByTile(_tilePos).x, GetPosByTile(_tilePos).y, 0.f));
	_obj->m_RoomNumber = m_Info.RoomNumber;

	if (!m_MainCam && LAYER_TYPE::Camera == _layr)
		m_MainCam = _obj;
}

void Room::DetachGameObject(GameObject* _obj)
{
	m_Layers[_obj->m_iLayerIdx]->DetachGameObject(_obj);
}

int Room::SetMainCam(GameObject* _cam)
{
	if (!_cam || !_cam->GetCamera())
		return E_FAIL;

	m_MainCam = _cam;

	return S_OK;
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

void Room::GetAllObject(std::vector<GameObject*>& _out)
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		if (!m_Layers[i]->m_Parents.empty())
		{
			_out.insert(_out.end(), m_Layers[i]->m_Parents.begin(), m_Layers[i]->m_Parents.end());
		}
	}
}
