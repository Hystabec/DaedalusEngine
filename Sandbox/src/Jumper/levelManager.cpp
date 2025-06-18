#include "levelManager.h"
#include "randomNumber.h"

namespace jumper
{

	LevelManager* LevelManager::s_instance = nullptr;

	LevelManager::LevelManager(const GameCamera& gameCam)
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

		// spawn all the starting platforms
		auto& camPos = gameCam.getPosition();
		auto& camBounds = gameCam.getBounds();

		m_initCamCorners[0] = { camBounds.left  - platformDespawnPadding,  camBounds.bottom - platformDespawnPadding, 0.0f };
		m_initCamCorners[1] = { camBounds.right + platformDespawnPadding,  camBounds.bottom - platformDespawnPadding, 0.0f };
		m_initCamCorners[2] = { camBounds.right + platformDespawnPadding,  camBounds.top    + platformDespawnPadding, 0.0f };
		m_initCamCorners[3] = { camBounds.left  - platformDespawnPadding,  camBounds.top    + platformDespawnPadding, 0.0f };

		for (int i = 0; i < 4; i++)
		{
			m_initCamCorners[i] = daedalusCore::maths::mat4::translate({ camPos.x, camPos.y, 0.0f })
				* daedalusCore::maths::mat4::scale({ 1.0f, 1.0f, 0.0f })
				* m_initCamCorners[i];
		}

		m_spawnGridBlockSize.x = (m_initCamCorners[1].x - m_initCamCorners[0].x) / spawnGridSize[0];
		m_spawnGridBlockSize.y = (m_initCamCorners[2].y - m_initCamCorners[0].y) / spawnGridSize[1];

		// need to ensure the centre platform is under the player
		// {0.0f, -0.125f}

		int currPlatIndex = 0;
		for (int x = 0; x < spawnGridSize[0]; x++)
		{
			for (int y = 0; y < spawnGridSize[1]; y++)
			{
				float xPos = m_initCamCorners[0].x + (m_spawnGridBlockSize.x * x) + (m_spawnGridBlockSize.x / 2.0f);
				float yPos = m_initCamCorners[0].y + (m_spawnGridBlockSize.y * y) + (m_spawnGridBlockSize.y / 2.0f);
				m_platforms[currPlatIndex].setGridCentre(daedalusCore::maths::vec2(xPos, yPos));

				//ensure the centre is always under the player at the start
				//if (x == 2 && y == 2)
				//	m_platforms[currPlatIndex].setPosition({ 0.0f, -0.125f });
				//else
				{
					float xOff = RandomNumber::randomRangeFloat(m_spawnGridBlockSize.x - (m_platforms[currPlatIndex].getScale().x / 2.0f), m_spawnGridBlockSize.x + (m_platforms[currPlatIndex].getScale().x / 2.0f));
					float yOff = RandomNumber::randomRangeFloat(m_spawnGridBlockSize.y - (m_platforms[currPlatIndex].getScale().y / 2.0f), m_spawnGridBlockSize.y + (m_platforms[currPlatIndex].getScale().y / 2.0f));

					m_platforms[currPlatIndex].setPosition({ xPos + xOff, yPos + yOff });

					//DD_LOG_INFO("Platform spawned at x: {} y: {} [centre point x: {} y: {}]", xPos + xOff, yPos + yOff, xPos, yPos);
				}

				currPlatIndex++;
			}
		}
	}

	void LevelManager::update(const JumperMan& character, const daedalusCore::maths::vec3& camPos)
	{
		// check if the platforms are still on the screen
		// if they are not "despawn" them
		daedalusCore::maths::vec3 translatedCamCorners[4];
		for (int i = 0; i < 4; i++)
		{
			translatedCamCorners[i] = daedalusCore::maths::mat4::translate({ camPos.x, camPos.y, 0.0f })
				* daedalusCore::maths::mat4::scale({ 1.0f, 1.0f, 0.0f })
				* m_initCamCorners[i];
		}

		int platformCount = 0;
		for (auto& platform : m_platforms)
		{
			//if (!platform.getActive())
			//	continue;

			// if a platform goes out of bounds respawn it on the opposite side of the camera bounds

			// Right of camera
			if ((platform.getPosition().x < translatedCamCorners[0].x))
			{
				platform.setPosition({ translatedCamCorners[1].x, platform.getPosition().y});
				//DD_LOG_TRACE("Platform moved [{}] (From right of camera)", platform.getPosition());
			}

			// Left of camera
			if ((platform.getPosition().x > translatedCamCorners[1].x))
			{
				platform.setPosition({ translatedCamCorners[0].x, platform.getPosition().y });
				//DD_LOG_TRACE("Platform moved [{}] (From left of camera)", platform.getPosition());
			}

			// Below the camera
			if ((platform.getPosition().y < translatedCamCorners[0].y))
			{
				platform.setPosition({ platform.getPosition().x,  translatedCamCorners[2].y});
				//DD_LOG_TRACE("Platform moved [{}] (From below the camera)", platform.getPosition());
			}

			/*
			// Above the camera
			if (!(platform.getPosition().y + platformDespawnPadding > cameraCorners[0].y))
			{
				DD_LOG_TRACE("Platform moved [{}] (From above the camera)", platform.getPosition());
			}
			*/

			platformCount++;
		}
	}

	void LevelManager::renderLevel()
	{
		for (auto& platform : m_platforms)
		{
			//if (!platform.getActive())
			//	continue;

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