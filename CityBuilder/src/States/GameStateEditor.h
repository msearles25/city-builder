#pragma once

#include "../Engine/GameState.h"
#include "../Engine/Map.h"

class GameStateEditor : public GameState
{
private:
	sf::View m_gameView;
	sf::View m_guiView;
	Map map;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};