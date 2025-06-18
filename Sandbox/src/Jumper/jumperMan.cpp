#include "jumperMan.h"
#include "levelManager.h"

#define DISABLE_JUMP false
#define USING_PARTICLE_SYSTEM true

namespace jumper
{

	JumperMan::JumperMan()
	{
		m_mainTexture = daedalusCore::graphics::Texture2D::create("resources/jumperAssets/jumperMan.png");
		m_graphicsProps.texture = m_mainTexture;
		m_graphicsProps.size = { spriteScale };

		m_currentJumpForce = jumpImpulse;

		m_jetpackPaticles.position = { 0.0f };
		m_jetpackPaticles.velocity = { 0.0f, -1.0f };
		m_jetpackPaticles.velocityVariation = { 0.5f, 2.0f };
		m_jetpackPaticles.sizeBegin = 0.025f;
		m_jetpackPaticles.sizeEnd = 0.0f;
		m_jetpackPaticles.sizeVariation = 0.0125f;
		m_jetpackPaticles.colourBegin = daedalusCore::utils::colour_vec4_to_normalized_vec4({255, 217, 0, 255});
		m_jetpackPaticles.colourEnd = daedalusCore::utils::colour_vec4_to_normalized_vec4({ 247, 15, 7, 255 });
		m_jetpackPaticles.lifeTime = 0.5f;
	}

	bool JumperMan::update(const daedalusCore::application::DeltaTime& dt)
	{
		m_time += dt;

		if (m_currentMaxHeightReached < m_graphicsProps.position.y)
			m_currentMaxHeightReached = m_graphicsProps.position.y;

		if (m_graphicsProps.position.y < -1.0f)
			return true;

		//DD_LOG_INFO("Score: {}", m_currentMaxHeightReached * 100.0f);

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

#if !DISABLE_JUMP
		if (LevelManager::get()->collisionCheck(*this) && m_currentJumpForce <= 0)
			m_currentJumpForce = jumpImpulse;
#else
		if (LevelManager::get()->collisionCheck(*this) && m_currentJumpForce <= 0)
			m_currentJumpForce = 0;
#endif

		m_graphicsProps.position += (daedalusCore::maths::vec3(0.0f, m_currentJumpForce, 0.0f) * (float)dt);

		m_currentJumpForce -= gravity * dt;

#if USING_PARTICLE_SYSTEM
		if (m_currentJumpForce > 0.0f)
		{
			// paricles
			if (m_time > m_particleNextEmitTime)
			{
				m_jetpackPaticles.position = { m_graphicsProps.position.x, m_graphicsProps.position.y - 0.07f };

				if (flipped)
					m_jetpackPaticles.position.x += 0.05f;
				else
					m_jetpackPaticles.position.x -= 0.05f;

				m_particleSystem.emit(m_jetpackPaticles);
				m_time = 0.0f;
			}
		}
		m_particleSystem.update(dt);
#endif

		return false;
	}

	void JumperMan::render() const
	{
#if USING_PARTICLE_SYSTEM
		m_particleSystem.render();
#endif

		daedalusCore::graphics::Renderer2D::drawQuad(m_graphicsProps);
	}

	void JumperMan::flipSprite(bool flipRight)
	{
		if (flipRight)
		{
			m_graphicsProps.size = { spriteScale };
			flipped = false;
		}
		else
		{
			m_graphicsProps.size = { -spriteScale, spriteScale };
			flipped = true;
		}
	}

}