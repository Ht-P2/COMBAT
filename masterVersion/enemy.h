
#pragma once

#include "gameObject.h"
#include "model.h"
#include <string>


enum ENEMY_STATE
{
	ENEMY_STATE_IDLE,
	ENEMY_STATE_TRACKING,
	ENEMY_STATE_JUMPATTACK,
	ENEMY_STATE_PUNCH01
};


class Enemy : public GameObject
{
private:
	ENEMY_STATE m_EnemyState = ENEMY_STATE_IDLE;

	class AnimationModel* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};


	D3DXVECTOR3 m_Velosity{};
	bool m_IsGround{};
	bool nowJump{};
	bool nowShake{};

	// アニメーション関連
	float m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	// 影
	class Shadow* m_Shadow{};

	// 角度（度）
	float m_Degree{};
	float m_KeepDegree{};

	// 追尾スピード
	float m_TrackingSpeed{};

	// JumpAttackのクールタイム
	float m_CoolTime{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// アニメーションのセット
	void SetAnimIdle();			// アイドル
	void SetAnimRun();			// ラン
	void SetAnimJumpAttack();	// ジャンプ攻撃
	void SetAnimPunch01();		// パンチ攻撃
	
	void UpdateIdle();			// 待機状態での更新処理
	void UpdateTracking();		// 追尾状態での更新処理
	void UpdateJumpAttack();	// ジャンプ攻撃時の更新処理
	void UpdatePunch01();		// 通常攻撃時の更新処理

	float Distance();			// player1とenemyの距離


	void SetGravity();			// 重力の設定
};