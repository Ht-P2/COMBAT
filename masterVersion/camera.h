
#pragma once

#include "gameObject.h"


class Camera : public GameObject
{
private:
	D3DXVECTOR3 m_Target{};
	D3DXMATRIX m_ViewMatrix{};
	D3DXMATRIX m_InverseViewMatrix;

	float m_ShakeAmplitube{};
	int m_ShakeTime{};
	float m_ShakeOffset{};

	// 角度（度）の変数
	float m_Degree = 0.0f;

public:
	D3DXVECTOR3 GetRight() {	// プレイヤーの右方向ベクトル取得
		D3DXMatrixInverse(&m_InverseViewMatrix, NULL, &m_ViewMatrix);

		D3DXVECTOR3 right;
		right.x = m_InverseViewMatrix._11;
		right.y = m_InverseViewMatrix._12;
		right.z = m_InverseViewMatrix._13;

		return right;
	}
	D3DXVECTOR3 GetTop() {		// プレイヤーの上方向ベクトル取得
		D3DXMatrixInverse(&m_InverseViewMatrix, NULL, &m_ViewMatrix);

		D3DXVECTOR3 top;
		top.x = m_InverseViewMatrix._21;
		top.y = m_InverseViewMatrix._22;
		top.z = m_InverseViewMatrix._23;

		return top;
	}
	D3DXVECTOR3 GetForward() {	// プレイヤーの前方向ベクトル取得
		D3DXMatrixInverse(&m_InverseViewMatrix, NULL, &m_ViewMatrix);

		D3DXVECTOR3 forward;
		forward.x = m_InverseViewMatrix._31;
		forward.y = m_InverseViewMatrix._32;
		forward.z = m_InverseViewMatrix._33;

		return forward;
	}


	void Init();
	void Update();
	void Draw();

	void UpdateGameCamera();
	void UpdateTitleCamera();


	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

	void Shake(float Amplitube)
	{
		m_ShakeAmplitube = Amplitube;
	}
};