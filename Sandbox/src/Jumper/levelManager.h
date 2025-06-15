#pragma once

#include <vector>
#include "platform.h"
#include "jumperMan.h"

namespace jumper
{

	class LevelManager
	{
	public:
		LevelManager();

		void update(const JumperMan& character);
		void renderLevel();

		bool collisionCheck(const JumperMan& character);

		inline static LevelManager* get() { return s_instance; };
	private:
		static LevelManager* s_instance;

		static const uint32_t platfromPoolSize = 12;
		std::vector<Platform> m_platforms;
		uint32_t m_currentSpawnIndex = 0;
	};

}
