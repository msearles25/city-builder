#include "City.h"

City::City()
{
	birthRate = 0.00055;
	deathRate = 0.00023;
	propCanWork = 0.50f;
	populationPool = 0;
	population = populationPool;
	employmentPool = 0;
	employable = employmentPool;
	residentialTax = 0.05;
	commercialTax = 0.05;
	industrialTax = 0.05;
	earnings = 0;
	funds = 0;
	currentTime = 0.0f;
	timePerDay = 1.0f;
	day = 0;
}

double City::getHomeless()
{
	return populationPool;
}

double City::getUnemployed()
{
	return employmentPool;
}
