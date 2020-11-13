#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "AnimationHandler.h"

enum class TileType
{
	VOID,
	GRASS,
	FOREST,
	WATER,
	RESIDENTIAL,
	COMMERCIAL,
	INDUSTRIAL,
	ROAD
};

std::string tileTypeToStr(TileType type);

class Tile
{
public:
	AnimationHandler m_animHandler;
	sf::Sprite m_sprite;
	
	TileType m_tileType;

	// Tile variants (for different looking versions of the same type)
	int m_tileVariant;

	// Region IDs of the tile. Tiles that are in the same region will be connected
	// the first is for transport
	unsigned int m_regions[1];

	// placement cost
	unsigned int m_cost;

	// Current residents / employees
	double m_population;
	// Max pop per growth stage/tile variant
	unsigned int m_maxPopPerLevel;
	// Mac number of building levels
	unsigned int m_maxLevels;
	// Production output per customer/worker a day. Monetary or goods
	float m_production;
	// Amount of goods
	float m_storedGoods;

	Tile();
	Tile(const unsigned int tileSize, const unsigned int height,
		sf::Texture& texture, const std::vector<Animation>& animations,
		const TileType tileType, const unsigned int cost, 
		const unsigned int maxPopPerLevel, const unsigned int maxLevels);

	void draw(sf::RenderWindow& window, float dt);
	void update();

	std::string getCost();
};