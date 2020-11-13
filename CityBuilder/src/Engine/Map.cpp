#include <fstream>
#include<map>
#include<string>
#include <vector>
#include "Map.h"
#include "Tile.h"

void Map::depthFirstSearch(std::vector<TileType>& whiteList, sf::Vector2i pos, int label, int regionType)
{
	if (pos.x < 0 || pos.x >= m_width) return;
	if (pos.y < 0 || pos.y >= m_height) return;

	if (m_tiles[pos.y * m_width + pos.x].m_regions[regionType] != 0) return;

	bool found{ false };

	for (auto type : whiteList)
	{
		if (type == m_tiles[pos.y * m_width + pos.x].m_tileType)
		{
			found = true;
			break;
		}
	}

	if (!found) return;

	m_tiles[pos.y * m_width + pos.x].m_regions[regionType] = label;

	depthFirstSearch(whiteList, pos + sf::Vector2i(-1, 0), label, regionType);
	depthFirstSearch(whiteList, pos + sf::Vector2i(0, 1), label, regionType);
	depthFirstSearch(whiteList, pos + sf::Vector2i(1, 0), label, regionType);
	depthFirstSearch(whiteList, pos + sf::Vector2i(0, -1), label, regionType);
}

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
	for (int y{ 0 }; y < m_height; ++y)
	{
		for (int x{ 0 }; x < m_width; ++x)
		{
			// Set the position of the tile in the world
			sf::Vector2f pos;
			pos.x = (x - y) * m_tileSize + m_width * m_tileSize;
			pos.y = (x + y) * m_tileSize * 0.5f;
			m_tiles[y * m_width + x].m_sprite.setPosition(pos);

			m_tiles[y * m_width + x].draw(window, dt);
		}
	}
}

void Map::findConnectedRegions(std::vector<TileType> whiteList, int regionType)
{
	int regions{ 1 };

	for (auto& tile : m_tiles) tile.m_regions[regionType] = 0;

	for (int y{ 0 }; y < m_height; ++y)
	{
		for (int x{ 0 }; x < m_width; ++x)
		{
			bool found{ false };
			for (auto type : whiteList)
			{
				if (type == m_tiles[y * m_width + x].m_tileType)
				{
					found = true;
					break;
				}
			}
			if (m_tiles[y * m_width + x].m_regions[regionType] == 0 && found)
			{
				depthFirstSearch(whiteList, sf::Vector2i(x, y), regions++, regionType);
			}
		}
	}
	m_numRegions[regionType] = regions;
}

void Map::updateDirection(TileType tileType)
{
	for (int y{ 0 }; y < m_height; ++y)
	{
		for (int x{ 0 }; x < m_width; ++x)
		{
			int pos{ int(y * m_width + x) };

			if (m_tiles[pos].m_tileType != tileType) continue;

			bool adjacentTiles[3][3]{ {0,0,0},{0,0,0},{0,0,0} };

			// Check for adjacent tiles of the same type
			if (x > 0 && y > 0)
				adjacentTiles[0][0] = (m_tiles[(y - 1) * m_width + (x - 1)].m_tileType == tileType);
			if (y > 0)
				adjacentTiles[0][1] = (m_tiles[(y - 1) * m_width + (x)].m_tileType == tileType);
			if (x < m_width - 1 && y > 0)
				adjacentTiles[0][2] = (m_tiles[(y - 1) * m_width + (x + 1)].m_tileType == tileType);
			if (x > 0)
				adjacentTiles[1][0] = (m_tiles[(y)*m_width + (x - 1)].m_tileType == tileType);
			if (x < m_width - 1)
				adjacentTiles[1][2] = (m_tiles[(y)*m_width + (x + 1)].m_tileType == tileType);
			if (x > 0 && y < m_height - 1)
				adjacentTiles[2][0] = (m_tiles[(y + 1) * m_width + (x - 1)].m_tileType == tileType);
			if (y < m_height - 1)
				adjacentTiles[2][1] = (m_tiles[(y + 1) * m_width + (x)].m_tileType == tileType);
			if (x < m_width - 1 && y < m_height - 1)
				adjacentTiles[2][2] = (m_tiles[(y + 1) * m_width + (x + 1)].m_tileType == tileType);


			if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
				m_tiles[pos].m_tileVariant = 2;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
				m_tiles[pos].m_tileVariant = 7;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
				m_tiles[pos].m_tileVariant = 8;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
				m_tiles[pos].m_tileVariant = 9;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
				m_tiles[pos].m_tileVariant = 10;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2])
				m_tiles[pos].m_tileVariant = 0;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1])
				m_tiles[pos].m_tileVariant = 1;
			else if (adjacentTiles[2][1] && adjacentTiles[1][0])
				m_tiles[pos].m_tileVariant = 3;
			else if (adjacentTiles[0][1] && adjacentTiles[1][2])
				m_tiles[pos].m_tileVariant = 4;
			else if (adjacentTiles[1][0] && adjacentTiles[0][1])
				m_tiles[pos].m_tileVariant = 5;
			else if (adjacentTiles[2][1] && adjacentTiles[1][2])
				m_tiles[pos].m_tileVariant = 6;
			else if (adjacentTiles[1][0])
				m_tiles[pos].m_tileVariant = 0;
			else if (adjacentTiles[1][2])
				m_tiles[pos].m_tileVariant = 0;
			else if (adjacentTiles[0][1])
				m_tiles[pos].m_tileVariant = 1;
			else if (adjacentTiles[2][1])
				m_tiles[pos].m_tileVariant = 1;
		}
	}
}

Map::Map()
{
	m_tileSize = 0;
	m_width = 0;
	m_height = 0;
	m_numRegions[0] = 1;
}

Map::Map(const std::string& filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas)
{
	m_tileSize = 0;
	load(filename, width, height, tileAtlas);
}