
#pragma once

#include "scene.h"


class Title : public Scene
{
private:
	class Audio* m_BGM{};
	class Fade* m_Fade{};

	static bool m_LoadFinish;

public:

	void Init() override;
	void Uninit() override;
	void Update() override;

};
