#include "DXUT.h"
#include <random>

float GetRealRandom(float min, float max)
{
	random_device rn;
	mt19937_64 rnd(rn());

	std::uniform_real_distribution<float> range(min, max);

	return range(rnd);
}

int GetIntRandom(int min, int max)
{
	random_device rn;
	mt19937_64 rnd(rn());

	std::uniform_int_distribution<int> range(min, max);

	return range(rnd);
}