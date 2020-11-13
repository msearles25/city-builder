#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class TextureManager
{
private:
	std::map<std::string, sf::Texture> m_textures;
public:
	void loadTexture(const std::string& name, const std::string& filename);
	sf::Texture& getRef(const std::string& texture);

	TextureManager();
};