#include "particleSystem.h"

#include "randomNumber.h"

namespace jumper::psystem
{
	static float lerp(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	static daedalusCore::maths::vec4 lerp(const daedalusCore::maths::vec4& a, const daedalusCore::maths::vec4& b, float t)
	{
		return a * (1.0f - t) + b * t;
	}


	ParticleSysyem::ParticleSysyem()
	{
		m_particlePool.resize(1000);
	}

	void ParticleSysyem::emit(const PartProps& particleProps)
	{
		Particle& part = m_particlePool[m_poolIndex];
		part.active = true;
		part.position = particleProps.position;
		part.rotation = RandomNumber::getRandomFloat() * 2.0f * 3.1415f;

		part.velocity = particleProps.velocity;
		part.velocity.x = particleProps.velocityVariation.x * (RandomNumber::getRandomFloat() - 0.5f);
		part.velocity.y = particleProps.velocityVariation.y * (RandomNumber::getRandomFloat() - 0.5f);

		part.colourBegin = particleProps.colourBegin;
		part.colourEnd = particleProps.colourEnd;

		part.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (RandomNumber::getRandomFloat() - 0.5f);
		part.sizeEnd = particleProps.sizeEnd;

		part.lifeTime = particleProps.lifeTime;
		part.lifeRemaing = particleProps.lifeTime;
		
		m_poolIndex = --m_poolIndex % m_particlePool.size();
	}

	void ParticleSysyem::update(const daedalusCore::application::DeltaTime& dt)
	{
		for (auto& part : m_particlePool)
		{
			if (!part.active)
				continue;

			if (part.lifeRemaing <= 0.0f)
			{
				part.active = false;
				continue;
			}

			part.lifeRemaing -= dt;
			part.position += part.velocity * (float)dt;
			part.rotation += 0.01f * (float)dt;
		}
	}

	void ParticleSysyem::render() const
	{
		for (auto& part : m_particlePool)
		{
			if (!part.active)
				continue;

			float life = part.lifeRemaing / part.lifeTime;
			daedalusCore::maths::vec4 colour = lerp(part.colourEnd, part.colourBegin, life);
			colour.w = colour.w * life;

			float size = lerp(part.sizeEnd, part.sizeBegin, life);
			daedalusCore::graphics::Renderer2D::drawRotatedQuad({ part.position, {size}, part.rotation, colour });
		}
	}

}