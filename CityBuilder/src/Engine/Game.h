#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include "GameState.h"

class Game
{
public:
	std::stack<GameState*> states;

	sf::RenderWindow window;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	Game();
	~Game();
};