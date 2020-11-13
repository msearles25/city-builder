#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Map
{
private:
	void depthFirstSearch(std::vector<TileType>& whiteList, sf::Vector2f pos, int label, int type);
public:
	unsigned int m_width;
	unsigned int m_height;

	std::vector<Tile> m_tiles;

	// Resource map
	std::vector<int> m_resources;

	unsigned int m_tileSize;
	unsigned int m_numSelected;
	unsigned int m_numRegions[1];

	// Load the map from disk
	void load(const std::string& filename, unsigned int width, 
		unsigned int height, std::map<std::string, Tile>& tileAtals);

	// Save the map to disk
	void save(const std::string& filename);

	// draw the ma
	void draw(sf::RenderWindow& window, float dt);

	/*
	Checks if one position is in the map is connected to another by only traversing tiles in the whitelist
	*/
	void findConnectedRegions(std::vector<TileType> whiteList, int type);

	/*
	Update the direction of the directional tiles so they face the correct way. Roads, pylons, rivers etc
	*/
	void updateDirection(TileType tileType);

	Map();
	Map(const std::string& filename, unsigned int width, 
		unsigned int height, std::map<std::string, Tile>& tileAtlas);
};