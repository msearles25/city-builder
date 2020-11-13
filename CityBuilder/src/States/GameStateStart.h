#pragma once

#include "../Engine/GameState.h"

class GameStateStart : public GameState
{
private:
	sf::View m_view;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateStart(Game* game);
};