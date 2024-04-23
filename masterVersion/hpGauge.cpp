
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "hpGauge.h"
#include "sprite.h"
#include "input.h"
#include "fade.h"
#include "result.h"

void HPGauge::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(20, 20, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(320, 20, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(20, 50, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(320, 50, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\white.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}


void HPGauge::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void HPGauge::Update()
{
	if (m_Current <= 0.0f)
	{
		m_Current = 0.0f;

		Scene* scene = Manager::GetScene();
		m_Fade = scene->GetGameObject<Fade>();

		m_Fade->FadeOut();

		if (m_Fade->GetFadeFinish())
		{
			Manager::SetScene<Result>();
		}
	}

	// HP�����̎Z�o
	m_Percentage = m_Current / m_Max;

	// �F�̕ύX
	if (m_Percentage > 0.5f)
		m_Color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// ��
	else if (m_Percentage > 0.3f)
		m_Color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	//���F
	else
		m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	//�ԐF
}


void HPGauge::Draw()
{
	//�@���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(20.0f, SCREEN_HEIGHT - 50.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(20.0f + (m_Percentage * m_Max), SCREEN_HEIGHT - 50.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(20.0f, SCREEN_HEIGHT - 20.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(0.0f + (m_Percentage * m_Max), SCREEN_HEIGHT - 20.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	VERTEX_3D* lostVertex = vertex + 4;  // Additional set of vertices for lost HP
	lostVertex[0].Position = D3DXVECTOR3(20.0f + (m_Percentage * m_Max), SCREEN_HEIGHT - 50.0f, 0.0f);
	lostVertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lostVertex[0].Diffuse = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f); 
	lostVertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	lostVertex[1].Position = D3DXVECTOR3(20.0f + m_Max, SCREEN_HEIGHT - 50.0f, 0.0f);
	lostVertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lostVertex[1].Diffuse = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	lostVertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	lostVertex[2].Position = D3DXVECTOR3(m_Percentage * m_Max, SCREEN_HEIGHT - 20.0f, 0.0f);
	lostVertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lostVertex[2].Diffuse = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	lostVertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	lostVertex[3].Position = D3DXVECTOR3(m_Max, SCREEN_HEIGHT - 20.0f, 0.0f);
	lostVertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lostVertex[3].Diffuse = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	lostVertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);



	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���b�N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(8, 0);
}


float HPGauge::SetDamage(float damage)
{
	m_Current -= damage;
	return m_Current;
}
