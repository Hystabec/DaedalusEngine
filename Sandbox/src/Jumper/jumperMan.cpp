#include "jumperMan.h"
#include "levelManager.h"

namespace jumper
{

	JumperMan::JumperMan()
	{
		m_mainTexture = daedalusCore::graphics::Texture2D::create("resources/jumperAssets/jumperMan.png");
		m_graphicsProps.texture = m_mainTexture;
		m_graphicsProps.size = { spriteScale };

		m_currentJumpForce = jumpImpulse;
	}

	void JumperMan::update(const daedalusCore::application::DeltaTime& dt)
	{
		if (m_currentMaxHeightReached < m_graphicsProps.position.y)
			m_currentMaxHeightReached = m_graphicsProps.position.y;

		//DD_LOG_INFO("Score: {}", m_currentMaxHeightReached);

		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_A))
		{
			m_graphicsProps.position.x -= movementSpeed * dt;
			flipSprite(false);
		}
		if (daedalusCore::application::Input::getKeyDown(DD_INPUT_KEY_D))
		{
			m_graphicsProps.position.x += movementSpeed * dt;
			flipSprite(true);
		}

		// if touching a platform reset jump force
		/*
		* for all the platforms in the level check if the player is touching any of them,
		* then check if the player is currently falling,
		* if both are TRUE reset the jump force
		*/
		//DD_LOG_TRACE("Collision Check: {}", LevelManager::get()->collisionCheck(*this));
		if (LevelManager::get()->collisionCheck(*this) && m_currentJumpForce <= 0)
			m_currentJumpForce = jumpImpulse;

		m_graphicsProps.position += (daedalusCore::maths::vec3(0.0f, m_currentJumpForce, 0.0f) * (float)dt);

		m_currentJumpForce -= gravity * dt;
	}

	void JumperMan::render() const
	{
		daedalusCore::graphics::Renderer2D::drawQuad(m_graphicsProps);
	}

	void JumperMan::flipSprite(bool flipRight)
	{
		if (flipRight)
			m_graphicsProps.size = { spriteScale };
		else
			m_graphicsProps.size = { -spriteScale, spriteScale };
	}

}