
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "player.h"
#include "particle.h"
#include "enemy.h"
#include "titleObj.h"


Model* Particle::m_Model{};

void Particle::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\bullet.obj");
}

void Particle::Unload()
{
	m_Model->Unload();
	delete m_Model;
}


void Particle::Init()
{
	Scene* scene = Manager::GetScene();
	TitleOBJ* title = scene->GetGameObject<TitleOBJ>();

	m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_Position = title->GetPosition() + D3DXVECTOR3(0.0f, -5.0f, -5.0f);
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.1f);


	vx = (float)(rand() % 15 - 5);
	vz = (float)(rand() % 15 + 5);
	ax = 0.0f;
	az = -0.9f;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
}


void Particle::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Particle::Update()
{
	Scene* scene = Manager::GetScene();
	TitleOBJ* title = scene->GetGameObject<TitleOBJ>();
	auto enemies = scene->GetGameObjects<Enemy>();

	pPos = title->GetPosition();

	vx += ax;
	vz += az;
	m_Position.x += vx * 0.2f;
	m_Position.z += vz * 0.2f;

	m_Scale += D3DXVECTOR3(0.27f, 0.27f, 0.27f);



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
}


void Particle::Draw()
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