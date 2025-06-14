#include "levelManager.h"

namespace jumper
{

	LevelManager::LevelManager()
	{
		auto platformTexture = daedalusCore::graphics::Texture2D::create("resources/jumperAssets/platfromTexture.png");
		for (int i = 0; i < platfromPoolSize; i++)
		{
			m_platforms.push_back(Platform(platformTexture));
		}

		for (int i = 0; i < platfromPoolSize; i++)
		{
			m_platforms[i].setPosition(daedalusCore::maths::vec2( i, 0 ));
		}
	}

	void LevelManager::renderLevel()
	{
		for (auto& platform : m_platforms)
			platform.render();
	}

}