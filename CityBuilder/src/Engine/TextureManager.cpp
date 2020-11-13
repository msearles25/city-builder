#include "TextureManager.h"

void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
	sf::Texture texture;
	texture.loadFromFile(filename);

	m_textures[name] = texture;
}

sf::Texture& TextureManager::getRef(const std::string& texture)
{
	return m_textures.at(texture);
}

TextureManager::TextureManager()
{

}
