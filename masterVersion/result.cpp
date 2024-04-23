
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "result.h"

#include "resultLogo.h"
#include "title.h"
#include "fade.h"


void Result::Init()
{
	AddGameObject<ResultLogo>(2);
	m_Fade = AddGameObject<Fade>(2);
}

void Result::Update()
{
	Scene::Update();

	// resultËtitle‚Ö‚Ì‰æ–Ê‘@ˆÛ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Title>();
	}
}