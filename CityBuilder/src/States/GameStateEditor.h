#pragma once

#include "../Engine/GameState.h"
#include "../Engine/Map.h"

enum class ActionState
{
	NONE,
	PANNING
};

class GameStateEditor : public GameState
{
private:
	ActionState actionState;

	sf::View m_gameView;
	sf::View m_guiView;

	Map map;

	sf::Vector2i panningAnchor;
	float zoomLevel;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};