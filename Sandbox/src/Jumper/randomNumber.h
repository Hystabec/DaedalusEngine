#pragma once

namespace RandomNumber
{
	void init();

	/// @brief Generate a random float between 0 and 1
	float getRandomFloat();

	/// @brief Generate a random number in the range min and max inclusive
	int randomRange(int min, int max);
};