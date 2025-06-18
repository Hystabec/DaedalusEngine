#pragma once

#include <Daedalus.h>
#include <vector>

namespace jumper::psystem
{

	struct PartProps
	{
		daedalusCore::maths::vec2 position;
		daedalusCore::maths::vec2 velocity, velocityVariation;
		daedalusCore::maths::vec4 colourBegin, colourEnd;
		float sizeBegin, sizeEnd, sizeVariation;
		float lifeTime = 1.0f;
	};

	class ParticleSysyem
	{
	public:
		ParticleSysyem();

		void emit(const PartProps& particleProps);
		void update(const daedalusCore::application::DeltaTime& dt);
		void render() const;

	private:
		struct Particle
		{
			daedalusCore::maths::vec2 position;
			daedalusCore::maths::vec2 velocity;
			daedalusCore::maths::vec4 colourBegin, colourEnd;
			float rotation = 0.0f;
			float sizeBegin, sizeEnd;

			float lifeTime = 1.0f;
			float lifeRemaing = 0.0f;

			bool active = false;
		};
		std::vector<Particle> m_particlePool;
		uint32_t m_poolIndex = 999;
	};

}