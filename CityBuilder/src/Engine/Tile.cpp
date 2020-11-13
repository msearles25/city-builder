#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(const unsigned int tileSize, const unsigned int height, 
	sf::Texture& texture, const std::vector<Animation>& animations, 
	const TileType tileType, const unsigned int cost, 
	const unsigned int maxPopPerLevel, const unsigned int maxLevels)
{
	m_tileType = tileType;
	m_tileVariant = 0;
	m_regions[0] = 0;

	m_cost = cost;
	m_population = 0;
	m_maxPopPerLevel = maxPopPerLevel;
	m_maxLevels = maxLevels;
	m_production = 0;
	m_storedGoods = 0;

	m_sprite.setOrigin(sf::Vector2f(0.f, tileSize * (height - 1)));
	m_sprite.setTexture(texture);
	m_animHandler.m_frameSize = sf::IntRect(0, 0, tileSize * 2, tileSize * height);

	for (auto animation : animations)
	{
		m_animHandler.addAnim(animation);
	}

	m_animHandler.update(0.f);
}

std::string Tile::getCost()
{
	// Return aa string that contains the cost of the tile
	return std::to_string(m_cost);
}
