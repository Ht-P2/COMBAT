
#include "main.h"
#include "renderer.h"
#include "resultLogo.h"
#include "sprite.h"



void ResultLogo::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


	// ��L�̃v���O��������s�ōs���Ă���
	AddComponent<Sprite>()->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\result.png");
}


void ResultLogo::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


	GameObject::Uninit();
}


void ResultLogo::Update()
{
	GameObject::Update();
}


void ResultLogo::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���b�N�X�ݒ�
	Renderer::SetWorldViewProjection2D();


	GameObject::Draw();
}