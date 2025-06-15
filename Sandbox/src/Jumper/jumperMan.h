#pragma once
#include <Daedalus.h>

namespace jumper {

	class JumperMan
	{
	public:
		JumperMan();

		void update(const daedalusCore::application::DeltaTime& dt);

		void render() const;

		inline void setPosition(const daedalusCore::maths::vec3& newPos) { m_graphicsProps.position = newPos; }
		inline const daedalusCore::maths::vec3& getPosition() const { return m_graphicsProps.position; }
		inline const daedalusCore::maths::vec2& getScale() const { return m_graphicsProps.size; }

	private:
		void flipSprite(bool flipRight);

	private:
		daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_mainTexture;
		daedalusCore::graphics::primatives2D::QuadProperties m_graphicsProps;

		const float spriteScale = 0.25f;
		const float gravity = 0.5f;
		const float jumpImpulse = 1.0f;
		const float movementSpeed = 1.0f;

		float m_currentJumpForce = 0.0f;
		float m_currentMaxHeightReached = 0.0f;
	};

}