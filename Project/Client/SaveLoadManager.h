#pragma once

class GameObject;

class SaveLoadManager
{
public:
	static void SaveGameObject(GameObject* _obj, FILE* _file);
	static GameObject* LoadGameObject(FILE* _file);
};

