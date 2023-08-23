#include "Random.h"

std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator

int randNumber(int min, int max) {
	std::uniform_int_distribution<> distr(min, max);

	return distr(gen);
}

float randNumberDouble(double min, double max)
{
	std::uniform_real_distribution<double> distr(min, max);

	//std::cout << distr(gen) << std::endl;
	return distr(gen);
}

float roundFloat(float var)
{
	// rounds float to two decimal places
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}
