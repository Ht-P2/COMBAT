
#pragma once
#include "gameObject.h"
#include "model.h"

class Particle : public GameObject
{
private:
	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};


	D3DXVECTOR3			m_Velocity{};			// 速度
	D3DXVECTOR3			pPos{};

	float	px, pz;		// 位置
	float	vx, vz;		// 速度
	float	ax, az;		// 加速度

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
};