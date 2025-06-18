#include "levelManager.h"

namespace jumper
{

	LevelManager* LevelManager::s_instance = nullptr;

	LevelManager::LevelManager()
	{
		DD_ASSERT(!s_instance, "Duplicate Level Managers");
		s_instance = this;

		m_platforms.reserve(platfromPoolSize);

		auto platformTexture = daedalusCore::graphics::Texture2D::create("resources/jumperAssets/platfromTexture.png");
		for (int i = 0; i < platfromPoolSize; i++)
		{
			m_platforms.emplace_back(platformTexture);
			m_platforms.back().setPosition({0.0f, -25.0f});
		}

		m_platforms[0].setPosition({0.0f, -0.125f});
		m_platforms[0].setActive(true);
		m_currentSpawnIndex = 1;
	}

	void LevelManager::update(const JumperMan& character, const GameCamera& gameCam)
	{
		// check if the platforms are still on the screen
		// if they are not "despawn" them

		auto& camPos = gameCam.getPosition();
		auto& camBounds = gameCam.getBounds();

		daedalusCore::maths::vec4 cameraCorners[4] = {
			{camBounds.left, camBounds.bottom, 0.0f, 1.0f},
			{camBounds.right, camBounds.bottom, 0.0f, 1.0f},
			{camBounds.right, camBounds.top, 0.0f, 1.0f },
			{camBounds.left, camBounds.top, 0.0f, 1.0f}
		};

		for (int i = 0; i < 4; i++)
		{
			cameraCorners[i] = daedalusCore::maths::mat4::translate({camPos.x, camPos.y, 0.0f})
				* daedalusCore::maths::mat4::scale({ 1.0f, 1.0f, 0.0f })
				* cameraCorners[i];

			//m_boundSquares.emplace_back(cameraCorners[i].x, cameraCorners[i].y);
		}

		// check if platforms are still in the spawn zone
		for (auto& platform : m_platforms)
		{
			if (!platform.getActive())
				continue;

			if (!(platform.getPosition().x - platformDespawnPadding < cameraCorners[1].x &&
				platform.getPosition().x + platform.getScale().x + platformDespawnPadding > cameraCorners[0].x &&
				platform.getPosition().y - platformDespawnPadding < cameraCorners[2].y &&
				platform.getPosition().y + platform.getScale().y + platformDespawnPadding > cameraCorners[0].y))
			{
				platform.setActive(false);
				DD_LOG_TRACE("Platform despawned (No longer on screen)");
			}
		}
	}

	void LevelManager::renderLevel()
	{
		for (auto& platform : m_platforms)
		{
			if (!platform.getActive())
				continue;

			platform.render();
		}

		/*for (auto& camPnt : m_boundSquares)
			daedalusCore::graphics::Renderer2D::drawQuad({ { camPnt.x, camPnt.y, 0.5f }, {0.1f, 0.1f}, {1.0f, 0.0f, 0.0f, 1.0f} });

		m_boundSquares.clear();*/
	}

	bool LevelManager::collisionCheck(const JumperMan& character)
	{
		daedalusCore::maths::vec4 playerVerts[4] = {
			{-0.5f,-0.5f, 0.0f, 1.0f},
			{0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, 0.5f, 0.0f, 1.0f },
			{-0.5f, 0.5f, 0.0f, 1.0f}
		};
		auto& charPos = character.getPosition();
		auto& charScale = character.getScale();
		for (int i = 0; i < 4; i++)
		{
			playerVerts[i] = daedalusCore::maths::mat4::translate({ charPos.x, charPos.y, 0.0f })
				* daedalusCore::maths::mat4::scale({ charScale.x, charScale.y, 0.0f })
				* playerVerts[i];
		}

		for (auto& curPlat : m_platforms)
		{
			daedalusCore::maths::vec4 platformVerts[4] = {
				{-0.5f,-0.5f + 0.25f, 0.0f, 1.0f},
				{0.5f, -0.5f + 0.25f, 0.0f, 1.0f},
				{0.5f, 0.5f - 0.375f, 0.0f, 1.0f },
				{-0.5f, 0.5f - 0.375f, 0.0f, 1.0f}
			};

			for (int i = 0; i < 4; i++)
			{
				platformVerts[i] = daedalusCore::maths::mat4::translate(curPlat.getPosition())
					* daedalusCore::maths::mat4::scale({ curPlat.getScale().x, curPlat.getScale().y, 1.0f })
					* platformVerts[i];
			}

			if (playerVerts[0].x < platformVerts[1].x &&
				playerVerts[1].x > platformVerts[0].x &&
				playerVerts[0].y < platformVerts[2].y &&
				playerVerts[2].y > platformVerts[0].y)
			{
				return true;
			}
		}

		return false;
	}

}