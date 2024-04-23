
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "enemyHP.h"
#include "explosion.h"

Model* Bullet::m_Model{};

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\bullet.obj");
}

void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}


void Bullet::Init()
{
	m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
}




void Bullet::Uninit()
{
	

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Bullet::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	Enemy* enemy = scene->GetGameObject<Enemy>();
	EnemyHP* enemyHP = scene->GetGameObject<EnemyHP>();


	//auto enemies = scene->GetGameObjects<Enemy>();
	
	
	pPos = player->GetPosition();
	m_Position += m_Velocity;

	// 座標の更新
	{
		m_Position.x += m_Velocity.x;
		m_Position.y = 1.0f;
		m_Position.z += m_Velocity.z;
	}

	// 弾が範囲外に行った場合に消す予約をする。
	if (m_Position.z > pPos.z + 20.0f) 
	{
		SetDestroy();
	}
	else if (m_Position.z < pPos.z - 20.0f)
	{
		SetDestroy();
	}
	else if (m_Position.x > pPos.x + 20.0f)
	{
		SetDestroy();
	}
	else if (m_Position.x < pPos.x - 20.0f)
	{
		SetDestroy();
	}
		
	// 敵との衝突判定
	{
		D3DXVECTOR3 position = enemy->GetPosition();
		D3DXVECTOR3 right = enemy->GetRight();
		D3DXVECTOR3 forward = enemy->GetForward();
		D3DXVECTOR3 direction = m_Position - position;

		float obbx = D3DXVec3Dot(&direction, &right);
		float obbz = D3DXVec3Dot(&direction, &forward);

		// 移動制御
		if (fabs(obbx) <= 2.0f && fabs(obbz) <= 2.0f)
		{
			// 弾の削除
			SetDestroy();

			// エフェクトの表示
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position + D3DXVECTOR3(0.0f, 0.8f, 0.0f));

			enemyHP->SetDamage(10);

			//m_Position = m_PreviousPosition;
		}
	}
}


void Bullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}