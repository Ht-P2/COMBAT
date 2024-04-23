
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"

#include "camera.h"
#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "audio.h"
#include "fade.h"
#include "meshField.h"
#include "particle.h"
#include "title.h"
#include "loading.h"
#include "titleObj.h"


MeshField* titleField;



void Title::Init()
{
	Particle::Load();
	
	// カメラ
	AddGameObject<Camera>(0);

	// フィールド
	titleField = AddGameObject<MeshField>(1);

	// プレイヤー
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(5.0f, -300.0f, 0.0f));

	// エネミー
	Enemy* enemy = AddGameObject<Enemy>(1);
	enemy->SetPosition(D3DXVECTOR3(0.0f, -300.0f, 0.0f));
	enemy->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// タイトルロゴ
	TitleOBJ* title = AddGameObject<TitleOBJ>(1);
	title->SetPosition(D3DXVECTOR3(0.0f, 20.0f, -5.0f));
	title->SetScale(D3DXVECTOR3(4.0f, 4.0f, 4.0f));
	title->SetRotation(D3DXVECTOR3(0.0f, 1.57f, 1.57f));

	// フェード
	m_Fade = AddGameObject<Fade>(2);

	
	// BGM
	/*m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\bgm\\battle.wav");
	m_BGM->Play(true);*/
}

void Title::Uninit()
{
	Scene::Uninit();

	Particle::Unload();
}

void Title::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	camera->UpdateTitleCamera();

	// パーティクル
	Particle* particle = AddGameObject<Particle>(1);

	// 波
	titleField->SetWave();

	// game⇒loadingへの画面繊維
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Loading>();
	}
}
