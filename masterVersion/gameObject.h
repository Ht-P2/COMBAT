
#pragma once


#include "component.h"
#include <list>

class GameObject
{
protected:
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Component*> m_Component;


	bool m_Destroy = false;			// �����\������邽�߂̃t���O

public:
	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }



	D3DXVECTOR3 GetRight() {	// �v���C���[�̉E�����x�N�g���擾
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}
	D3DXVECTOR3 GetTop() {		// �v���C���[�̏�����x�N�g���擾
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 top;
		top.x = rot._21;
		top.y = rot._22;
		top.z = rot._23;

		return top;
	}
	D3DXVECTOR3 GetForward() {	// �v���C���[�̑O�����x�N�g���擾
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}



	// ���z�֐�
	virtual void Init() {}

	virtual void Uninit() 
	{
		// �͈�for���[�v
		for (Component* componet : m_Component)
		{
			componet->Uninit();
			delete componet;
		}

		m_Component.clear();
	}

	virtual void Update() 
	{
		// �͈�for���[�v
		for (Component* componet : m_Component)
		{
			componet->Update();
		}
	}

	virtual void Draw() 
	{
		// �͈�for���[�v
		for (Component* componet : m_Component)
		{
			componet->Draw();
		}
	}


	// �e���v���[�g�֐�
	template <typename T>
	T* AddComponent()
	{
		Component* componet = new T();
		m_Component.push_back(componet);
		componet->Init();

		return (T*)componet;
	}
};