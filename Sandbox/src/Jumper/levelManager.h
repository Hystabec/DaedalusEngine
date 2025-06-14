#pragma once

#include <vector>
#include "platform.h"

namespace jumper
{

	class LevelManager
	{
	public:
		LevelManager();

		void renderLevel();

	private:
		static const uint32_t platfromPoolSize = 12;
		std::vector<Platform> m_platforms;
		uint32_t currentSpawnIndex = 0;
	};

}
