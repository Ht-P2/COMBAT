
#pragma once

#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>



class Scene
{
protected:
	std::list<GameObject*> m_GameObject[3];	// STLの構造
											// リスト構造を３つに

public:
	virtual void Init(){}

	virtual void Uninit()
	{
		for (int i = 0; i < 3; i++)
		{
			// 範囲forループ
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
			// 範囲forループ
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}
			// 消す予約があるものをリスト構造から削除
			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });	// ラムダ式
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			// 範囲forループ
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
	}


	// テンプレート関数
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
				if (typeid(*object) == typeid(T))	// 型を調べる(RTTI動的型情報)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}
	// typeidはゲームエンジン向きじゃない


	// 複数のオブジェクトの取得
	template<typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;	// stlの配列

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
