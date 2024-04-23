
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "loading.h"
#include "loadingLogo.h"
#include "game.h"
#include "fade.h"

#include <thread>

void Loading::Init()
{
	AddGameObject<LoadingLogo>(2);

	std::thread th(&Game::Load);
	th.detach();
}

void Loading::Update()
{
	Scene::Update();

	// titleËgame‚Ö‚Ì‰æ–Ê‘@ˆÛ
	if (Game::GetLoadFinish())
	{
		Manager::SetScene<Game>();
	}
}