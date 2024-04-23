
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"

#include "camera.h"
#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "result.h"
#include "audio.h"
#include "sky.h"
#include "fade.h"
#include "meshField.h"
#include "hpGauge.h"
#include "enemyHP.h"


MeshField* meshField;
bool Game::m_LoadFinish = false;


void Game::Load()
{
	Bullet::Load();

	m_LoadFinish = true;
}

void Game::Unload()
{
	m_LoadFinish = false;

	Bullet::Unload();
}

void Game::Init()
{
	// カメラ
	AddGameObject<Camera>(0);

	// 空
	AddGameObject<Sky>(1);

	// フィールド
	meshField = AddGameObject<MeshField>(1);

	// プレイヤー
	Player* player = AddGameObject<Player>(1); 
	player->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));

	// エネミー
	Enemy* enemy = AddGameObject<Enemy>(1);
	enemy->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 7.0f));
	enemy->SetRotation(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	
	// フェード
	m_Fade = AddGameObject<Fade>(2);

	// HPゲージ
	AddGameObject<HPGauge>(2);
	AddGameObject<EnemyHP>(2);

	// BGM
	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\bgm\\battle.wav");
	m_BGM->Play(true);
}

void Game::Uninit()
{
	Scene::Update();
}

void Game::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	Enemy* enemy = scene->GetGameObject<Enemy>();
	Player* player = scene->GetGameObject<Player>();

	// カメラ制御
	camera->UpdateGameCamera();

	// 重力セット
	enemy->SetGravity();
	player->SetGravity();


	// game⇒resultへの画面繊維
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Result>();
	}
}