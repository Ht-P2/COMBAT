
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

	// �A�j���[�V�����֘A
	float m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName;
	std::string m_NextAnimationName;

	// �e
	class Shadow* m_Shadow{};

	// �p�x�i�x�j
	float m_Degree{};
	float m_KeepDegree{};

	// �ǔ��X�s�[�h
	float m_TrackingSpeed{};

	// JumpAttack�̃N�[���^�C��
	float m_CoolTime{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// �A�j���[�V�����̃Z�b�g
	void SetAnimIdle();			// �A�C�h��
	void SetAnimRun();			// ����
	void SetAnimJumpAttack();	// �W�����v�U��
	void SetAnimPunch01();		// �p���`�U��
	
	void UpdateIdle();			// �ҋ@��Ԃł̍X�V����
	void UpdateTracking();		// �ǔ���Ԃł̍X�V����
	void UpdateJumpAttack();	// �W�����v�U�����̍X�V����
	void UpdatePunch01();		// �ʏ�U�����̍X�V����

	float Distance();			// player1��enemy�̋���


	void SetGravity();			// �d�͂̐ݒ�
};