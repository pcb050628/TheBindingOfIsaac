#include "pch.h"
#include "SaveLoadManager.h"

#include <Engine\GameObject.h>
#include <Engine\Script.h>

#include <Scripts\ScriptManager.h>

void SaveLoadManager::SaveGameObject(GameObject* _obj, FILE* _file)
{
	//오브젝트 이름
	SaveWString(_obj->GetName(), _file);

	//오브젝트 컴포넌트
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		bool isExist = false;
		Component* comp = _obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr != comp)
			isExist = true;

		fwrite(&isExist, sizeof(bool), 1, _file);

		if (isExist)
		{
			comp->SaveToFile(_file);
		}
	}

	//오브젝트 스크립트
	vector<Script*> scripts = _obj->GetScript();
	size_t scriptCount = scripts.size();
	fwrite(&scriptCount, sizeof(size_t), 1, _file);

	for (size_t i = 0; i < scriptCount; i++)
	{
		SaveWString(ScriptManager::GetScriptName(scripts[i]), _file);
		scripts[i]->SaveToFile(_file);
	}

	std::vector<GameObject*> child = _obj->GetChild();
	size_t childCount = child.size();
	fwrite(&childCount, sizeof(size_t), 1, _file);

	for (size_t i = 0; i < childCount; i++)
	{
		SaveGameObject(child[i], _file);
	}
}

GameObject* SaveLoadManager::LoadGameObject(FILE* _file)
{
	GameObject* obj = new GameObject;

	//오브젝트 이름
	wstring name;
	LoadWString(name, _file);
	obj->SetName(name);

	//오브젝트 컴포넌트
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		bool isExist = false;
		fread(&isExist, sizeof(bool), 1, _file);

		if (isExist)
		{
			Component* comp = GetComponentByComponentType((COMPONENT_TYPE)i);
			comp->LoadFromFile(_file);
			obj->AddComponent(comp);
		}
	}

	//오브젝트 스크립트
	size_t scriptCount = 0;
	fread(&scriptCount, sizeof(size_t), 1, _file);

	for (size_t i = 0; i < scriptCount; i++)
	{
		wstring scriptName;
		LoadWString(scriptName, _file);

		Script* scrpt = ScriptManager::GetScript(scriptName);
		scrpt->LoadFromFile(_file);
	}

	size_t childCount = 0;
	fwrite(&childCount, sizeof(size_t), 1, _file);

	for (size_t i = 0; i < childCount; i++)
	{
		GameObject* childobj = LoadGameObject(_file);
		obj->AttachChild(childobj);
	}

	return obj;
}
