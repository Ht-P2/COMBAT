
#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "sprite.h"


void Fade::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");


	m_Sprite = AddComponent<Sprite>();
	m_Sprite ->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\black.png");
}


void Fade::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


	GameObject::Uninit();
}


void Fade::Update()
{
	GameObject::Update();

	if (!m_FadeOut)					// �t�F�[�h�C��
	{
		m_Alpha -= 0.01f;
		if (m_Alpha < 0.0f) 
		{
			m_Alpha = 0.0f;
		}
	}
	else if (m_FadeOut)				// �t�F�[�h�A�E�g
	{
		m_Alpha += 0.01f;
		if (m_Alpha > 1.0f) 
		{
			m_Alpha = 1.0f;
			m_FadeFinish = true;
		}
	}

	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}


void Fade::Draw()
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