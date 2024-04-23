
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

	if (!m_FadeOut)					// フェードイン
	{
		m_Alpha -= 0.01f;
		if (m_Alpha < 0.0f) 
		{
			m_Alpha = 0.0f;
		}
	}
	else if (m_FadeOut)				// フェードアウト
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
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリックス設定
	Renderer::SetWorldViewProjection2D();


	GameObject::Draw();
}