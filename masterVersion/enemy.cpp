
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "animationModel.h"
#include "input.h"
#include "scene.h"
#include "camera.h"
#include "meshField.h"
#include "player.h"
#include "hpGauge.h"


void Enemy::Init()
{
	GameObject::Init();

	// �A�j���[�V�����̃��[�h
	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\enemyModel.fbx");
	m_Model->LoadAnimation("asset\\model\\enemyIdle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\enemyRun.fbx", "Run"); 
	m_Model->LoadAnimation("asset\\model\\enemyJump.fbx", "JumpAttack");
	m_Model->LoadAnimation("asset\\model\\enemyPunch01.fbx", "Punch01");
	

	// �A�j���[�V���������ݒ�
	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	// �X�P�[���̐ݒ�
	m_Scale = (D3DXVECTOR3(0.03f, 0.03f, 0.03f));

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	// �ǐՃX�s�[�h�ݒ�
	m_TrackingSpeed = 0.05f;
}


void Enemy::Uninit()
{
	GameObject::Uninit();

	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Enemy::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>();
	Camera* camera = scene->GetGameObject<Camera>();
	Player* player = scene->GetGameObject<Player>();

	// �v���C���̔���
	D3DXVECTOR3 playerPos = scene->GetGameObject<Player>()->GetPosition();
	D3DXVECTOR3 directionToPlayer = playerPos - m_Position;
	D3DXVec3Normalize(&directionToPlayer, &directionToPlayer);

	// �v���C���[�Ɍ���
	m_Rotation.y = atan2(directionToPlayer.x, directionToPlayer.z);


	// ��]����
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

	

	// �J�����̐U������
	if (nowShake == true)
	{
		camera->Shake(0.5f);			// �J�����̐U��
		player->SetPlayerShake(true);	// �v���C���[�̈ړ��X�^��
	}
	if (m_Time > 125.0f)
	{
		nowShake = false;
		player->SetPlayerShake(false);	// �v���C���[�̃X�^������
	}
	

	// StatePattern
	switch (m_EnemyState)
	{
	case ENEMY_STATE_IDLE:
		UpdateIdle();
		break;

	case ENEMY_STATE_TRACKING:
		UpdateTracking();
		break;

	case ENEMY_STATE_JUMPATTACK:
		UpdateJumpAttack();
		break;

	case ENEMY_STATE_PUNCH01:
		UpdatePunch01();
		break;

	default:
		break;
	}

	m_CoolTime += 1;
}


void Enemy::Draw()
{
	GameObject::Draw();

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	Renderer::SetWorldMatrix(&world);


	// �A�j���[�V����
	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);

	m_Time += 0.7f;

	m_BlendRate += 0.1f;

	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0f;
	}

	m_Model->Draw();

}



void Enemy::SetAnimIdle()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "Idle";
	m_BlendRate = 0.0f;
}

void Enemy::SetAnimRun()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "Run";
	m_BlendRate = 0.0f;
}

void Enemy::SetAnimJumpAttack()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "JumpAttack";
	m_BlendRate = 0.0f;
}

void Enemy::SetAnimPunch01()
{
	m_AnimationName = m_NextAnimationName;
	m_NextAnimationName = "Punch01";
	m_BlendRate = 0.0f;
}


void Enemy::UpdateIdle()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	MeshField* meshField = scene->GetGameObject<MeshField>();

	SetAnimIdle();

	// �t�B�[���h�̃��Z�b�g
	meshField->Init();

	// �ǔ���Ԃɑ@��
	if (Distance() >= 5.0f)
	{
		m_EnemyState = ENEMY_STATE_TRACKING;
	}
}

void Enemy::UpdateTracking()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	// Run�A�j���[�V�����̃Z�b�g
	SetAnimRun();

	// �v���C���[��ǔ�
	D3DXVECTOR3 playerPos = scene->GetGameObject<Player>()->GetPosition();
	D3DXVECTOR3 directionToPlayer = playerPos - m_Position;
	D3DXVec3Normalize(&directionToPlayer, &directionToPlayer);

	// �����A�ړ�
	m_Rotation.y = atan2(directionToPlayer.x, directionToPlayer.z);
	m_Position += directionToPlayer * m_TrackingSpeed;

	// ��苗�����ꂽ��X�^���U��
	if (Distance() > 10.0f && m_CoolTime > 500)
	{
		if (m_NextAnimationName != "JumpAttack")
		{
			SetAnimJumpAttack();
			m_Time = 0.0f;
			m_CoolTime = 0.0f;
			m_EnemyState = ENEMY_STATE_JUMPATTACK;
		}
	}

	// ��苗�����ɓ�������p���`�U��
	if (Distance() <= 5.5f)
	{
		if (m_NextAnimationName != "Punchi01")
		{
			SetAnimPunch01();
			m_Time = 0.0f;
			m_EnemyState = ENEMY_STATE_PUNCH01;
		}
	}
}

void Enemy::UpdateJumpAttack()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	MeshField* meshField = scene->GetGameObject<MeshField>();

	meshField->SetWave();
	nowShake = true;

	// Idle�ɑ@��
	if (m_Time > 54)
	{
		m_EnemyState = ENEMY_STATE_IDLE;
	}
}

void Enemy::UpdatePunch01()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	HPGauge* hp = scene->GetGameObject<HPGauge>();

	bool playerAvoid = player->GetAvoid();

	
	// Idle�ɑ@��
	if (m_Time > 85)
	{
		m_EnemyState = ENEMY_STATE_IDLE;
	}
	else if (40.0f < m_Time && m_Time < 41.0f)
	{
		// ����t���O��OFF�̏ꍇ�̂�
		if (playerAvoid == false)
			hp->SetDamage(20.0f);
	}
}


float Enemy::Distance()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 playerPos = scene->GetGameObject<Player>()->GetPosition();
	D3DXVECTOR3 distanceVector = playerPos - m_Position;

	return D3DXVec3Length(&distanceVector);
}

void Enemy::SetGravity()
{
	Scene* scene = Manager::GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>();

	// �d��
	m_Velosity.y -= 0.015f;
	// �ړ�
	m_Position += m_Velosity;

	// ���b�V���t�B�[���h�Ƃ̏Փ˔���
	float groundHeight = 0.0f;
	groundHeight = meshField->GetHeight(m_Position);

	// �ݒu
	if (m_Position.y < groundHeight && m_Velosity.y < 0.0f)
	{
		m_IsGround = true;
		m_Position.y = groundHeight;
		m_Velosity.y = 0.0f;
		nowJump = false;
	}
	else
	{
		m_IsGround = false;
	}
}
