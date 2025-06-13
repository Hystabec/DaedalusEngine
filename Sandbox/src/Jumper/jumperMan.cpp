#include "jumperMan.h"

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
		// if touching a platform reset jump force

		if (m_currentJumpForce <= (-jumpImpulse))
			m_currentJumpForce = jumpImpulse;

		m_graphicsProps.position += (daedalusCore::maths::vec3(0.0f, m_currentJumpForce, 0.0f) * (float)dt);

		m_currentJumpForce -= gravity * dt;
	}

	void JumperMan::render() const
	{
		daedalusCore::graphics::Renderer2D::drawQuad(m_graphicsProps);
	}

}