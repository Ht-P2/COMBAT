
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
	// �J����
	AddGameObject<Camera>(0);

	// ��
	AddGameObject<Sky>(1);

	// �t�B�[���h
	meshField = AddGameObject<MeshField>(1);

	// �v���C���[
	Player* player = AddGameObject<Player>(1); 
	player->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -5.0f));

	// �G�l�~�[
	Enemy* enemy = AddGameObject<Enemy>(1);
	enemy->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 7.0f));
	enemy->SetRotation(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	
	// �t�F�[�h
	m_Fade = AddGameObject<Fade>(2);

	// HP�Q�[�W
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

	// �J��������
	camera->UpdateGameCamera();

	// �d�̓Z�b�g
	enemy->SetGravity();
	player->SetGravity();


	// game��result�ւ̉�ʑ@��
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Fade->FadeOut();
	}

	if (m_Fade->GetFadeFinish())
	{
		Manager::SetScene<Result>();
	}
}