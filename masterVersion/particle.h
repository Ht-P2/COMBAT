
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


	D3DXVECTOR3			m_Velocity{};			// ���x
	D3DXVECTOR3			pPos{};

	float	px, pz;		// �ʒu
	float	vx, vz;		// ���x
	float	ax, az;		// �����x

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
};