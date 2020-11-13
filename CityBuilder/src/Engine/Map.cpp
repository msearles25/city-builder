#include <fstream>
#include<map>
#include<string>
#include <vector>
#include "Map.h"
#include "Tile.h"

void Map::load(const std::string& filename, unsigned int width, 
	unsigned int height, std::map<std::string, Tile>& tileAtals)
{
	std::ifstream inputFile;
	inputFile.open(filename, std::ios::in | std::ios::binary);

	m_width = width;
	m_height = height;

	for (int pos{ 0 }; pos < width * height; ++pos)
	{
		m_resources.push_back(255);

		TileType tileType;
		inputFile.read((char*)&tileType, sizeof(int));

		switch (tileType)
		{
		case TileType::VOID:
		case TileType::GRASS:
			m_tiles.push_back(tileAtals.at("grass"));
			break;
		case TileType::FOREST:
			m_tiles.push_back(tileAtals.at("forest"));
			break;
		case TileType::WATER:
			m_tiles.push_back(tileAtals.at("water"));
			break;
		case TileType::RESIDENTIAL:
			m_tiles.push_back(tileAtals.at("residential"));
			break;
		case TileType::COMMERCIAL:
			m_tiles.push_back(tileAtals.at("commercial"));
			break;
		case TileType::INDUSTRIAL:
			m_tiles.push_back(tileAtals.at("industrial"));
			break;
		case TileType::ROAD:
			m_tiles.push_back(tileAtals.at("road"));
			break;
		default:
			break;
		}

		Tile& tile{ m_tiles.back() };
		inputFile.read((char*)&tile.m_tileVariant, sizeof(int));
		inputFile.read((char*)&tile.m_regions, sizeof(int)*1);
		inputFile.read((char*)&tile.m_population, sizeof(double));
		inputFile.read((char*)&tile.m_storedGoods, sizeof(float));
	}

	inputFile.close();
}

void Map::save(const std::string& filename)
{
	std::ofstream outputFile;
	outputFile.open(filename, std::ios::out | std::ios::binary);

	for (auto tile : m_tiles)
	{
		outputFile.write((char*)&tile.m_tileType, sizeof(int));
		outputFile.write((char*)&tile.m_tileVariant, sizeof(int));
		outputFile.write((char*)&tile.m_regions, sizeof(int) * 1);
		outputFile.write((char*)&tile.m_population, sizeof(double));
		outputFile.write((char*)&tile.m_storedGoods, sizeof(float));
	}

	outputFile.close();
}

void Map::draw(sf::RenderWindow& window, float dt)
{
}

void Map::findConnectedRegions(std::vector<TileType> whiteList, int type)
{
}

void Map::updateDirection(TileType tileType)
{
}

Map::Map()
{
	m_tileSize = 0;
	m_width = 0;
	m_height = 0;
	m_numRegions[0] = 1;
}

Map::Map(const std::string& filename, , unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas)
{
	m_tileSize = 0;
	load(filename, width, height, tileAtlas);
}