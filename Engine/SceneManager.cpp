#include "SceneManager.h"
#include"global.h"
#include"../TestScene.h"
SceneManager::SceneManager(GameObject* parent) :GameObject(parent, "SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = SCENE_ID_TEST;
	Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	/*シーンを切り替え
	現在と次が別だったら切り替え
	現在のシーンの後片付け
	新しいシーンの準備*/
	if (currentSceneID_ != nextSceneID_)
	{
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();
		
		switch (nextSceneID_) {
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this); break;
		
		}
		currentSceneID_ = nextSceneID_;

	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}
/// <summary>
/// 指定したシーンをnextSceneID_に設定する（＝次のUpdateが呼ばれたときに切り替わる）
/// </summary>
/// <param name="_next"></param>
void SceneManager::ChangeScene(SCENE_ID _next)
{
	nextSceneID_ = _next;
}