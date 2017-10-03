#include <Misc\RandomGenerator.h>
#include <random>
#include <math.h>
using namespace Logic;

int RandomGenerator::getRandomInt(int from, int to)
{
	return (rand() % std::abs(to - from)) + from;
}

float RandomGenerator::getRandomFloat(float from, float to)
{
	// f is [0, 1]
	float f = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
	// return [from, to]
	return (f * std::abs(to - from)) + from - to;
}