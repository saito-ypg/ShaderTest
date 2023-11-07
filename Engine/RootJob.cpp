#include"RootJob.h"
#include"SceneManager.h"

RootJob::RootJob(GameObject*parent):GameObject(parent,"RootJob")
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{	
	//Instantiate‚É‘ã‘Ö‚³‚ê‚½‚½‚ßˆÈ‰º‚Ìˆ—‚ÍÁ‚¦‚é
	/*PlayScene* pPlayScene;
	pPlayScene = new PlayScene(this);

	pPlayScene->Initialize();
	childList_.push_back(pPlayScene);
	*/
	Instantiate<SceneManager>(this);
}

void RootJob::Update()
{
}

void RootJob::Draw()
{
	
}

void RootJob::Release()
{
	
}
