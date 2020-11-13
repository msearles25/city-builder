#include "Map.h"

Map::Map()
{
	tileSize = 0;
	width = 0;
	height = 0;
	numRegions[0] = 1;
}

Map::Map(const std::string& filename, , unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas)
{
	tileSize = 0;
	load(filename, width, height, tileAtlas);
}