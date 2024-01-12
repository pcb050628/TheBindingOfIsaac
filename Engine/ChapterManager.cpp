#include "pch.h"
#include "ChapterManager.h"
#include "TaskManager.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "components.h"

ChapterManager::ChapterManager()
	: m_CurChapter(nullptr)
	, m_Chapters{}
{

}

ChapterManager::~ChapterManager()
{
	for (Chapter* chptr : m_Chapters)
	{
		delete chptr;
	}
}

void ChapterManager::Init()
{
	for (int i = 0; i < 4; i++)
	{
		m_Chapters[i] = new Chapter;
	}

	m_CurChapter = m_Chapters[0];

	GameObject* gobj = new GameObject;
	gobj->AddComponent(new Transform);
	gobj->AddComponent(new MeshRenderer);
	gobj->AddComponent(new Animator2D);
		
	gobj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"default_Material"));
	gobj->GetMeshRenderer()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"RectMesh"));
	//gobj->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture(ResourceManager::GetInst()->Find<Texture>(L"Rocks"), TEX_0);

	gobj->GetAnimator2D()->CreateAnim(L"test_anim", ResourceManager::GetInst()->Find<Texture>(L"Rocks"), Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), Vec2(40.f, 40.f), 6, 1);
	gobj->GetAnimator2D()->Play(L"test_anim", true);

	AddGameObject(gobj, LAYER_TYPE::Player);

	gobj = new GameObject;
	gobj->AddComponent(new Transform);
	gobj->AddComponent(new Camera);

	gobj->GetTransform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));

	gobj->GetCamera()->SetCameraPriority(0);
	gobj->GetCamera()->LayerCheck(LAYER_TYPE::Player, true);

	AddGameObject(gobj, LAYER_TYPE::Background);
}

void ChapterManager::Update()
{
	m_CurChapter->Update();
	m_CurChapter->LateUpdate();
}

void ChapterManager::Render()
{
	m_CurChapter->Render();
}

void ChapterManager::DetachGameObject(GameObject* _obj)
{
	m_CurChapter->DetachGameObject(_obj);
}

void ChapterManager::RegisterObj(GameObject* _obj, LAYER_TYPE _layerType)
{
	m_CurChapter->GetCurRoom()->GetLayer(_layerType)->RegisterObject(_obj);
}

