
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "bullet.h"
#include "meshField.h"
#include "enemy.h"

#include "audio.h"
#include "shadow.h"
#include "animationModel.h"


void Player::Init()
{
	// アニメーションのロード
	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\player02.fbx");
	m_Model->LoadAnimation("asset\\model\\playerRun.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\punchHit.fbx", "Hit");
	m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\avoidance.fbx", "Avoid");

	// 弾の初期座標
	m_bulletDefault = D3DXVECTOR3(0.0f, 0.8f, 0.0f);

	// アニメーション初期設定
	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	// スケールの設定
	m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	
	m_ShotSE = AddComponent<Audio>();
	m_ShotSE->Load("asset\\audio\\se\\shot.wav");


	m_Shadow = AddComponent<Shadow>();
}


void Player::Uninit()
{
	GameObject::Uninit();


	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Player::Update()
{
	GameObject::Update();


	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	MeshField* meshField = scene->GetGameObject<MeshField>();
	Enemy* enemy = scene->GetGameObject<Enemy>();
	//auto enemies = scene->GetGameObjects<Enemy>();

	D3DXVECTOR3 oldPosition = m_Position;


	// 弾の発射
	if (m_Position.y == 0)
	{
		if (Input::GetKeyTrigger(VK_UP))
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position + m_bulletDefault);
			bullet->SetVelocity(camera->GetForward() * 0.05f);

			m_ShotSE->Play();
		}
		if (Input::GetKeyTrigger(VK_DOWN))
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position + m_bulletDefault);
			bullet->SetVelocity(camera->GetForward() * -0.05f);

			m_ShotSE->Play();
		}
		if (Input::GetKeyTrigger(VK_RIGHT))
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position + m_bulletDefault);
			bullet->SetVelocity(camera->GetRight() * 0.05f);

			m_ShotSE->Play();
		}
		if (Input::GetKeyTrigger(VK_LEFT))
		{
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position + m_bulletDefault);
			bullet->SetVelocity(camera->GetRight() * -0.05f);

			m_ShotSE->Play();
		}
	}

	


	// 回転処理
	if (Input::GetKeyPress('Q'))
	{
		m_Degree -= 1.0f;
		m_KeepDegree = 0.0f;

	}
	else if (Input::GetKeyPress('E'))
	{
		m_Degree += 1.0f;
		m_KeepDegree = 0.0f;
	}

	// 当たり判定
	{
		D3DXVECTOR3 position = enemy->GetPosition();
		D3DXVECTOR3 right = enemy->GetRight();
		D3DXVECTOR3 forward = enemy->GetForward();
		D3DXVECTOR3 direction = m_Position - position;

		float obbx = D3DXVec3Dot(&direction, &right);
		float obbz = D3DXVec3Dot(&direction, &forward);

		// 移動制御
		if (fabs(obbx) <= 5.0f && fabs(obbz) <= 5.0f) 
		{

			m_Position = m_PreviousPosition;
		}
	}

	
	// StatePattern
	switch (m_PlayerState)
	{
	case PLAYER_STATE_GROUND:
		UpdateGround();
		break;

	case PLAYER_STATE_JUMP:
		UpdateJump();
		break;

	case PLAYER_STATE_HIT:
		UpdateHit();
		break;

	case PLAYER_STATE_AVOID:
		UpdateAvoid();
		break;

	default:
		break;
	}
}


void Player::Draw()
{
	GameObject::Draw();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);	//  + D3DX_PI モデルの向き調整
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	m_Matrix = world;


	Renderer::SetWorldMatrix(&world);

	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);

	//m_Time++;

	m_Time += 0.7f;

	m_BlendRate += 0.1f;

	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0f;
	}

	m_Model->Draw();
}

void Player::SetAnim()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "Run";
	m_BlendRate = 0.0f;
}

void Player::SetAnimHit()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "Hit";
	m_BlendRate = 0.0f;
}

void Player::SetAnimAvoid()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "Avoid";
	m_BlendRate = 0.0f;
}

void Player::UpdateGround()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	D3DXVECTOR3 cameraForward = camera->GetForward();
	cameraForward.y = 0.0f;
	D3DXVec3Normalize(&cameraForward, &cameraForward);

	m_PreviousPosition = m_Position;

	// 回避フラグOFF
	m_Avoid = false;

	// プレイヤー操作
	if(m_ShakeCamera == false)
	{
		bool move = false;

		// 後
		if (Input::GetKeyPress('S'))
		{
			if (m_NextAnimationName != "Run")
			{
				SetAnim();
			}

			if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
			{
				m_Position -= (camera->GetForward() * 0.1f) * 0.71f;
				m_KeepDegree = 180.0f;
			}
			else
			{
				m_Position -= camera->GetForward() * 0.1f;
				m_KeepDegree = 180.0f;
			}

			move = true;
		}
		// 前
		if (Input::GetKeyPress('W'))
		{
			if (m_NextAnimationName != "Run")
			{
				SetAnim();
			}

			if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
			{
				m_Position += (camera->GetForward() * 0.1f) * 0.71f;
				m_KeepDegree = 0.0f;
			}
			else
			{
				m_Position += camera->GetForward() * 0.1f;
				m_KeepDegree = 0.0f;
			}

			move = true;
		}
		// 左
		if (Input::GetKeyPress('A'))
		{
			if (m_NextAnimationName != "Run")
			{
				SetAnim();
			}

			if (Input::GetKeyPress('S'))
			{
				m_Position -= (camera->GetRight() * 0.1f) * 0.71f;
				m_KeepDegree += 45.0f;
			}
			else if (Input::GetKeyPress('W'))
			{
				m_Position -= (camera->GetRight() * 0.1f) * 0.71f;
				m_KeepDegree -= 45.0f;
			}
			else
			{
				m_Position -= camera->GetRight() * 0.1f;
				m_KeepDegree = -90.0f;
			}

			move = true;
		}
		// 右
		if (Input::GetKeyPress('D'))
		{
			if (m_NextAnimationName != "Run")
			{
				SetAnim();
			}

			if (Input::GetKeyPress('S'))
			{
				m_Position += (camera->GetRight() * 0.1f) * 0.71f;
				m_KeepDegree -= 45.0f;
			}
			else if (Input::GetKeyPress('W'))
			{
				m_Position += (camera->GetRight() * 0.1f) * 0.71f;
				m_KeepDegree += 45.0f;
			}
			else
			{
				m_Position += camera->GetRight() * 0.1f;
				m_KeepDegree = 90.0f;
			}

			move = true;
		}

		if (move == false)
		{
			if (m_NextAnimationName != "Idle")
			{
				m_AnimationName = m_NextAnimationName;
				m_NextAnimationName = "Idle";
				m_BlendRate = 0.0f;
			}
		}


		// プレイヤーの角度計算
		m_Rotation.y = (m_Degree + m_KeepDegree) * (D3DX_PI / 180.0f);
	}
	else
	{
		if (m_NextAnimationName != "Idle")
		{
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;
		}
	}


	// ジャンプフラグ
	// 回避Action追加のためコメントアウト
	/*if (nowJump == false)
	{
		// ジャンプ
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			m_Velosity.y = 0.35f;
			m_PlayerState = PLAYER_STATE_JUMP;
			nowJump = true;
		}
	}*/

	// プレイヤーダメージアニメーション(動作確認)
	if (Input::GetKeyPress('M'))
	{
		SetAnimHit();
		m_Time = 0.0f;
		m_PlayerState = PLAYER_STATE_HIT;
	}
	

	// 回避
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		// 向き、位置の更新
		m_KeepDegree = 0.0f;
		SetAnimAvoid();
		m_Time = 0.0f;
		m_PlayerState = PLAYER_STATE_AVOID;
	}

}

void Player::UpdateJump()
{
	if (m_IsGround == true)
	{
		m_PlayerState = PLAYER_STATE_GROUND;
	}
}

void Player::UpdateHit()
{
	if (m_Time > 46.0f)
	{
		m_PlayerState = PLAYER_STATE_GROUND;
	}
}

void Player::UpdateAvoid()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();


	// 回避フラグON
	m_Avoid = true;

	m_Position -= camera->GetForward() * 0.1f;

	if (m_Time > 20.0f)
	{
		m_PlayerState = PLAYER_STATE_GROUND;
	}
}


void Player::SetGravity()
{
	Scene* scene = Manager::GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>();

	// 重力
	m_Velosity.y -= 0.015f;
	// 移動
	m_Position += m_Velosity;

	// メッシュフィールドとの衝突判定
	m_GroundHeight = meshField->GetHeight(m_Position);

	// 設置
	if (m_Position.y < m_GroundHeight && m_Velosity.y < 0.0f)
	{
		m_IsGround = true;
		m_Position.y = m_GroundHeight;
		m_Velosity.y = 0.0f;
		nowJump = false;
	}
	else
	{
		m_IsGround = false;
	}

	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = m_GroundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
}
