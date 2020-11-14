#pragma once
#include <map>
#include <stack>
#include <string>
#include <SFML/Graphics.hpp>
#include "GUI.h"
#include "TextureManager.h"
#include "Tile.h"

class GameState;

class Game
{
private:
	void loadTexture();
	void loadTiles();
	void loadStyleSheets();
	void loadFonts();
public:
	const static int tileSize{ 8 };

	std::stack<GameState*> states;

	sf::RenderWindow window;
	TextureManager textureManager;
	sf::Sprite background;

	std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> m_styleSheets;
	std::map<std::string, sf::Font> m_fonts;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	Game();
	~Game();
};