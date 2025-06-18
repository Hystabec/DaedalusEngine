#pragma once

#include <vector>
#include "platform.h"
#include "jumperMan.h"
#include "gameCamera.h"

namespace jumper
{
	static const float platformDespawnPadding = 2.0f;
	static const uint32_t spawnGridSize[2] = {5, 5};
	static const uint32_t platfromPoolSize = spawnGridSize[0] * spawnGridSize[1];

	class LevelManager
	{
	public:
		LevelManager(const GameCamera& gameCam);

		void update(const JumperMan& character, const daedalusCore::maths::vec3& camPos);
		void renderLevel();

		bool collisionCheck(const JumperMan& character);

		inline static LevelManager* get() { return s_instance; };
	private:
		static LevelManager* s_instance;
		
		std::vector<Platform> m_platforms;

		uint32_t m_currentSpawnIndex = 0;
		daedalusCore::maths::vec3 m_initCamCorners[4];
		daedalusCore::maths::vec2 m_spawnGridBlockSize;
		//std::vector<daedalusCore::maths::vec2> m_boundSquares;
	};

}
