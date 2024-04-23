
#pragma once

#include "gameObject.h"


class HPGauge : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	float m_Max = 300.0f;			// 最大HP
	float m_Current = 300.0f;		// 現在のHP
	float m_Percentage;		// HPの割合		

	class Fade* m_Fade{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float SetDamage(float damage);
};