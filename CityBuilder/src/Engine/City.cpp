#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
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
	int width = 0;
	int height = 0;

	std::ifstream inputFile(cityName + "_cfg.dat", std::ios::in);
	std::string line;

	while (std::getline(inputFile, line))
	{
		std::istringstream lineStream(line);
		std::string key;

		if (std::getline(lineStream, key, '='))
		{
			std::string value;
			if (std::getline(lineStream, value))
			{
				if (key == "width") width = std::stoi(value);
				else if (key == "height") height = std::stoi(value);
				else if (key == "day") day = std::stoi(value);
				else if (key == "populationPool") populationPool = std::stod(value);
				else if (key == "employmentPool") employmentPool = std::stod(value);
				else if (key == "population") population = std::stod(value);
				else if (key == "employable") employable = std::stod(value);
				else if (key == "birthRate") birthRate = std::stod(value);
				else if (key == "deathRate") deathRate = std::stod(value);
				else if (key == "residentialTax") residentialTax = std::stod(value);
				else if (key == "commercialTax") commercialTax = std::stod(value);
				else if (key == "industrialTax") industrialTax = std::stod(value);
				else if (key == "funds") funds = std::stod(value);
				else if (key == "earnings") earnings = std::stod(value);
			}
			else
			{
				std::cerr << "Error, value for key " << key << '\n';
			}
		}
	}

	inputFile.close();

	map.load(cityName + "_map.dat", width, height, tileAtlas);
	tileChanged();
}

void City::save(std::string cityName)
{
	std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);

	outputFile << "width=" << map.m_width << '\n';
	outputFile << "height=" << map.m_height<< '\n';
	outputFile << "day=" << day << '\n';
	outputFile << "populationPool=" << populationPool << '\n';
	outputFile << "employmentPool=" << employmentPool << '\n';
	outputFile << "population=" << population << '\n';
	outputFile << "employable=" << employable << '\n';
	outputFile << "birthRate=" << birthRate << '\n';
	outputFile << "deathRate=" << deathRate << '\n';
	outputFile << "residentialTax=" << residentialTax << '\n';
	outputFile << "commercialTax=" << commercialTax << '\n';
	outputFile << "industrialTax=" << industrialTax << '\n';
	outputFile << "funds=" << funds << '\n';
	outputFile << "earnings=" << earnings << '\n';

	outputFile.close();

	map.save(cityName + "_map.dat");
}

void City::update(float dt)
{
}

void City::bulldoze(const Tile& tile)
{
	for (int pos{ 0 }; pos < map.m_width * map.m_height; ++pos)
	{
		if (map.m_selected[pos] == 1)
		{
			if (map.m_tiles[pos].m_tileType == TileType::RESIDENTIAL)
			{
				populationPool += map.m_tiles[pos].m_population;
			}
			else if (map.m_tiles[pos].m_tileType == TileType::COMMERCIAL)
			{
				employmentPool += map.m_tiles[pos].m_population;
			}
			else if (map.m_tiles[pos].m_tileType == TileType::INDUSTRIAL)
			{
				employmentPool += map.m_tiles[pos].m_population;
			}

			map.m_tiles[pos] = tile;
		}
	}
}

void City::shuffleTiles()
{
	while (shuffledTiles.size() < map.m_tiles.size())
	{
		shuffledTiles.push_back(0);
	}

	std::iota(shuffledTiles.begin(), shuffledTiles.end(), 1);

	std::random_shuffle(shuffledTiles.begin(), shuffledTiles.end());
}

void City::tileChanged()
{
	map.updateDirection(TileType::ROAD);
	map.findConnectedRegions(
		{
			TileType::ROAD, TileType::RESIDENTIAL,
			TileType::COMMERCIAL, TileType::INDUSTRIAL
		}, 0);
}

double City::getHomeless()
{
	return populationPool;
}

double City::getUnemployed()
{
	return employmentPool;
}
