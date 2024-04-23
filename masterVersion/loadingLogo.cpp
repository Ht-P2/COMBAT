
#include "main.h"
#include "renderer.h"
#include "loadingLogo.h"
#include "sprite.h"



void LoadingLogo::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


	// ��L�̃v���O��������s�ōs���Ă���
	AddComponent<Sprite>()->Init(0, 0, 200, 200, "asset\\texture\\loading.png");
}


void LoadingLogo::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


	GameObject::Uninit();
}


void LoadingLogo::Update()
{
	GameObject::Update();

	m_Position = D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
	m_Rotation.z += 0.01f;
}


void LoadingLogo::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���b�N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// �}�g���b�N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	GameObject::Draw();
}