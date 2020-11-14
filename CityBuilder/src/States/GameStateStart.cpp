#include <utility>
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

	for (auto gui : guiSystem) m_game->window.draw(gui.second);
}

void GameStateStart::update(const float dt)
{
}

void GameStateStart::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePos{ m_game->window.mapPixelToCoords(
		sf::Mouse::getPosition(m_game->window), m_view) };

	while (m_game->window.pollEvent(event))
	{
		switch (event.type)
		{
			// close the window
		case sf::Event::Closed:
		{
			m_game->window.close();
			break;
		}
			// Resize the window
		case sf::Event::Resized:
		{
			m_view.setSize(event.size.width, event.size.height);
			m_game->background.setPosition(m_game->window.mapPixelToCoords(sf::Vector2i(0, 0)));

			sf::Vector2f pos{ sf::Vector2f(event.size.width, event.size.height) };
			pos *= 0.5f;
			pos = m_game->window.mapPixelToCoords(sf::Vector2i(pos), m_view);

			guiSystem.at("menu").setPosition(pos);
	
			m_game->background.setScale(
				float(event.size.width) / float(m_game->background.getTexture()->getSize().x),
				float(event.size.height) / float(m_game->background.getTexture()->getSize().y));

			break;
		}
			// Check if key is pressed
		case sf::Event::MouseMoved:
		{
			guiSystem.at("menu").highlight(guiSystem.at("menu").getEntry(mousePos));
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::string msg{ guiSystem.at("menu").activate(mousePos) };

				if (msg == "load_game")
				{
					loadGame();
				}
			}
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape)
				m_game->window.close();
			break;
		}
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

	guiSystem.emplace("menu", GUI(sf::Vector2f(192, 32), 4, false, game->m_styleSheets.at("button"),
		{ std::make_pair("Load Game", "load_game") }));

	guiSystem.at("menu").setPosition(pos);
	guiSystem.at("menu").setOrigin(96, 32 * 1 / 2);
	guiSystem.at("menu").show();
}
