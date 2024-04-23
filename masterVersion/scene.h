
#pragma once

#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>



class Scene
{
protected:
	std::list<GameObject*> m_GameObject[3];	// STL�̍\��
											// ���X�g�\�����R��

public:
	virtual void Init(){}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			// �͈�for���[�v
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Uninit();
				delete gameObject;
			}

			m_GameObject[i].clear();
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			// �͈�for���[�v
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}
			// �����\�񂪂�����̂����X�g�\������폜
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });	// �����_��
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			// �͈�for���[�v
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
	}


	// �e���v���[�g�֐�
	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	template<typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))	// �^�𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}
	// typeid�̓Q�[���G���W����������Ȃ�


	// �����̃I�u�W�F�N�g�̎擾
	template<typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;	// stl�̔z��

		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}
};
