#include "randomNumber.h"

#include <random>

static std::mt19937 s_randomEngine;
static std::uniform_int_distribution<unsigned int> s_distrobution;

void RandomNumber::init()
{
	s_randomEngine.seed(std::random_device()());
}

float RandomNumber::getRandomFloat()
{
	return (float)s_distrobution(s_randomEngine) / (float)std::numeric_limits<uint32_t>::max();
}

int RandomNumber::randomRange(int min, int max)
{
	// the random float is being times by the max value 
	// so that it can be treated as a int value for the mod 
	// operator
	return ((int)(getRandomFloat() * max)%(int)(max - min + 1)) + min;
}

float RandomNumber::randomRangeFloat(float min, float max)
{
	float diff = max - min;
	return min + (getRandomFloat() * diff);
}
