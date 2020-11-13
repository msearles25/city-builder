#pragma once

#include "../Engine/GameState.h"
#include "../Engine/Map.h"

enum class ActionState
{
	NONE,
	PANNING,
	SELECTING
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

	sf::Vector2i selectionStart;
	sf::Vector2i selectionEnd;

	Tile* currentTile;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};