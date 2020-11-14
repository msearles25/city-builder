#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "City.h"
#include "Tile.h"

double City::distributePool(double& pool, Tile& tile, double rate)
{
	const static int moveRate{ 4 };
	unsigned int maxPop{ tile.m_maxPopPerLevel * (tile.m_tileVariant + 1) };

	if (pool > 0)
	{
		int moving{ maxPop - tile.m_population };
		if (moving > moveRate) moving = moveRate;
		if (pool - moving < 0) moving = pool;
		pool -= moving;
		tile.m_population += moving;
	}

	tile.m_population += tile.m_population * rate;

	if (tile.m_population > maxPop)
	{
		pool += tile.m_population - maxPop;
		tile.m_population = maxPop;
	}

	return tile.m_population;
}

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

void City::load(std::string cityName, std::map<std::string, Tile>& tileAtlas)
{
}

void City::save(std::string cityName)
{
}

void City::update(float dt)
{
}

void City::bulldoze(const Tile& tile)
{
}

void City::shuffleTiles()
{
}

void City::tileChanged()
{
}

double City::getHomeless()
{
	return populationPool;
}

double City::getUnemployed()
{
	return employmentPool;
}
