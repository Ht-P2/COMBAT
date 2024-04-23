
#pragma once
#include "gameObject.h"
#include "model.h"

class Bullet : public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader*	m_PixelShader{};
	ID3D11InputLayout*	m_VertexLayout{};

	D3DXVECTOR3			m_Velocity{};			// 速度

	D3DXVECTOR3			pPos{};

	D3DXVECTOR3 m_PreviousPosition{};	// 前フレームのポジション

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
};