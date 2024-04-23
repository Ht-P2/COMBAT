
#pragma once

#include "gameObject.h"
#include "model.h"
#include <string>


// プレイヤーの状態
enum PLAYER_STATE
{
	PLAYER_STATE_GROUND,
	PLAYER_STATE_JUMP,
	PLAYER_STATE_HIT,
	PLAYER_STATE_AVOID
};


class Player : public GameObject
{
private:

	class AnimationModel* m_Model{};
	class Audio* m_ShotSE{};
	class Shadow* m_Shadow{};

	PLAYER_STATE m_PlayerState = PLAYER_STATE_GROUND;
	
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXMATRIX m_Matrix;
	D3DXVECTOR3 m_PreviousPosition{};	// 前フレームのポジション

	D3DXVECTOR3 m_Velosity{};
	D3DXVECTOR3 m_bulletDefault{};		// 弾のデフォルト位置

	bool m_IsGround{};

	bool nowJump{};		// ジャンプ中か
	bool m_Gravity{};
	bool m_ShakeCamera{};
	bool m_Avoid{};

	// 角度（度）の変数
	float m_Degree{};
	float m_KeepDegree{};

	// 地面の高さ
	float m_GroundHeight{};


	float m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();



	void SetAnim();
	void SetAnimHit();
	void SetAnimAvoid();

	void UpdateGround();
	void UpdateJump();
	void UpdateHit();
	void UpdateAvoid();


	void SetGravity();
	bool SetPlayerShake(bool shake)
	{
		m_ShakeCamera = shake;
		return m_ShakeCamera;
	}
	
	D3DXMATRIX GetMatrix() { return m_Matrix; }


	bool SetAvoid(bool avoid)
	{
		m_Avoid = avoid;
		return m_Avoid;
	}

	bool GetAvoid() { return m_Avoid; }
};