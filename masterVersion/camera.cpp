
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "input.h"
#include "enemy.h"
#include "titleObj.h"

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


void Camera::Update()
{
	// カメラシェイク
	m_ShakeOffset = (float)sin(m_ShakeTime * 1.5f) * m_ShakeAmplitube;
	m_ShakeTime++;
	m_ShakeAmplitube *= 0.9f;
}


void Camera::Draw()
{
	// ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXVECTOR3 position = m_Position + D3DXVECTOR3(0.0f, m_ShakeOffset, 0.0f);
	D3DXVECTOR3 target = m_Target + D3DXVECTOR3(0.0f, m_ShakeOffset, 0.0f);

	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void Camera::UpdateGameCamera()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	// カメラ制御（トップビュー）
	m_Target = player->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_Rotation.y = m_Degree * (D3DX_PI / 180.0f);		// 度→ラジアン値　変換
	m_Position = m_Target + (D3DXVECTOR3(sinf(m_Rotation.y) * 4.0f, 1.0f, -cosf(m_Rotation.y) * 4.0f));

	if (Input::GetKeyPress('Q'))			// 左を向く
	{
		m_Degree += 1.0f;
	}
	else if (Input::GetKeyPress('E'))		// 右を向く
	{
		m_Degree -= 1.0f;
	}
}

void Camera::UpdateTitleCamera()
{
	Scene* scene = Manager::GetScene();
	TitleOBJ* TitleObj = scene->GetGameObject<TitleOBJ>();

	// カメラ制御（トップビュー）
	m_Target = TitleObj->GetPosition() + D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_Rotation.y = m_Degree * (D3DX_PI / 180.0f);		// 度→ラジアン値　変換
	m_Position = m_Target + (D3DXVECTOR3(sinf(m_Rotation.y) * 5.0f, 15.0f, -cosf(m_Rotation.y) * 5.0f));
}
