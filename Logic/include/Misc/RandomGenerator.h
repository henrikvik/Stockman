#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

namespace Logic
{
	class RandomGenerator
	{
		public:
			static RandomGenerator& singleton()
			{
				static RandomGenerator generator;
				return generator;
			}

			int getRandomInt(int from, int to);
			float getRandomFloat(float from, float to);
	};
}

#endif