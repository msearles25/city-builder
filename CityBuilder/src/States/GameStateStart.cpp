#include "../Engine/GameState.h"
#include "GameStateEditor.h"
#include "GameStateStart.h"

void GameStateStart::loadGame()
{
	m_game->pushState(new GameStateEditor(m_game));
}

void GameStateStart::draw(const float dt)
{
	m_game->window.setView(m_view);

	m_game->window.clear(sf::Color::Black);
	m_game->window.draw(m_game->background);
}

void GameStateStart::update(const float dt)
{
}

void GameStateStart::handleInput()
{
	sf::Event event;
	while (m_game->window.pollEvent(event))
	{
		switch (event.type)
		{
			// close the window
		case sf::Event::Closed:
			m_game->window.close();
			break;
			// Resize the window
		case sf::Event::Resized:
			m_view.setSize(event.size.width, event.size.height);
			m_game->background.setPosition(m_game->window.mapPixelToCoords(sf::Vector2i(0, 0)));
			m_game->background.setScale(
				float(event.size.width) / float(m_game->background.getTexture()->getSize().x),
				float(event.size.height) / float(m_game->background.getTexture()->getSize().y));
			break;
			// Check if key is pressed
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				m_game->window.close();
			else if (event.key.code == sf::Keyboard::Space)
				loadGame();
			break;
		default:
			break;
		}
	}
}

GameStateStart::GameStateStart(Game* game)
{
	m_game = game;
	
	sf::Vector2f pos{ sf::Vector2f(game->window.getSize()) };
	m_view.setSize(pos);

	pos *= 0.5f;

	m_view.setCenter(pos);
}
