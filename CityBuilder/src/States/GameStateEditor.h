#pragma once

#include <map>
#include <string>
#include "../Engine/City.h"
#include "../Engine/GameState.h"
#include "../Engine/GUI.h"
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

	//Map map;
	City city;

	sf::Vector2i panningAnchor;
	float zoomLevel;

	sf::Vector2i selectionStart;
	sf::Vector2i selectionEnd;

	Tile* currentTile;

	std::map<std::string, GUI> guiSystem;
public:
	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();

	GameStateEditor(Game* game);
};