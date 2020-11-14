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
		int moving{ int(maxPop - tile.m_population) };
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
	double popTotal{ 0 };
	double commercialRevenue{ 0 };
	double industrialRevenue{ 0 };

	// Update the game time
	currentTime += dt;

	if (currentTime < timePerDay) return;
	++day;

	currentTime = 0.0;
	if (day % 30 == 0)
	{
		funds += earnings;
		earnings = 0;
	}

	// Run first pass of tile updates. Handles pool distribution
	for (int i{ 0 }; i < map.m_tiles.size(); ++i)
	{
		Tile& tile{ map.m_tiles[shuffledTiles[i]] };

		if (tile.m_tileType == TileType::RESIDENTIAL)
		{
			// Redistribute pool and increase pop total by the tile's pop
			distributePool(populationPool, tile, birthRate - deathRate);

			popTotal += tile.m_population;
		}
		else if (tile.m_tileType == TileType::COMMERCIAL)
		{
			// Hire
			if (rand() % 100 < 15 * (1.0 - commercialTax))
			{
				distributePool(employmentPool, tile, 0.00);
			}
		}
		else if (tile.m_tileType == TileType::INDUSTRIAL)
		{
			// Extract resources from ground
			if (map.m_resources[i] > 0 && rand() % 100 < population)
			{
				++tile.m_production;
				--map.m_resources[i];
			}

			// Hire
			if (rand() % 100 < 15 * (1.0 - industrialTax))
			{
				distributePool(employmentPool, tile, 0.00);
			}
		}

		tile.update();
	}

	// Goods manufacturing
	for (int i{ 0 }; i < map.m_tiles.size(); ++i)
	{
		Tile& tile{ map.m_tiles[shuffledTiles[i]] };

		if (tile.m_tileType == TileType::INDUSTRIAL)
		{
			int recievedResources{ 0 };

			for (auto& tile2 : map.m_tiles)
			{
				if (tile2.m_regions[0] == tile.m_regions[0] && tile2.m_tileType == TileType::INDUSTRIAL)
				{
					if (tile2.m_production > 0)
					{
						++recievedResources;
						--tile2.m_production;
					}
					if (recievedResources >= tile.m_tileVariant + 1) break;
				}
			}

			// turn resources into goods
			tile.m_storedGoods += (recievedResources + tile.m_production) * (tile.m_tileVariant + 1);
		}
	}

	// Goods distribution
	for (int i{ 0 }; i < map.m_tiles.size(); ++i)
	{
		Tile& tile{ map.m_tiles[shuffledTiles[i]] };

		if (tile.m_tileType == TileType::COMMERCIAL)
		{
			int recievedGoods{ 0 };
			double maxCustomers{ 0.0 };

			for (auto& tile2 : map.m_tiles)
			{
				if (tile2.m_regions[0] == tile.m_regions[0] &&
					tile2.m_tileType == TileType::INDUSTRIAL &&
					tile2.m_storedGoods > 0)
				{
					while (tile2.m_storedGoods > 0 && recievedGoods != tile.m_tileVariant + 1)
					{
						--tile2.m_storedGoods;
						++recievedGoods;
						industrialRevenue += 100 * (1.0 - industrialTax);
					}
				}
				else if (tile2.m_regions[0] == tile.m_regions[0] &&
					tile2.m_tileType == TileType::RESIDENTIAL)
				{
					maxCustomers += tile2.m_population;
				}
				if (recievedGoods == tile.m_tileVariant + 1) break;
			}

			tile.m_production = (recievedGoods * 100.0 + rand() % 20) * (1.0 - commercialTax);

			double revenue{ tile.m_production * maxCustomers * tile.m_population / 100.0 };

			commercialRevenue += revenue;
		}
	}

	// Adjust population pool for deaths/births
	populationPool += populationPool * (birthRate - deathRate);
	popTotal += populationPool;

	// Adjust employment pool
	float newWorkers{ (popTotal - population) * propCanWork };	
	newWorkers *= newWorkers < 0 ? -1 : 1;

	employmentPool += newWorkers;
	employable += newWorkers;

	if (employmentPool < 0)employmentPool = 0;
	if (employable < 0) employable = 0;
	
	// update city pop
	population = popTotal;

	// calc city income from the tax
	earnings = (population - populationPool) * 15 * residentialTax;
	earnings += commercialRevenue * commercialTax;
	earnings += industrialRevenue * industrialTax;
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
