#include "GameStateEditor.h"

void GameStateEditor::draw(const float dt)
{
	m_game->window.clear(sf::Color::Black);

	m_game->window.setView(m_guiView);
	m_game->window.draw(m_game->background);

	m_game->window.setView(m_gameView);
	map.draw(m_game->window, dt);
}

void GameStateEditor::update(const float dt)
{

}

void GameStateEditor::handleInput()
{
	sf::Event event;

	while (m_game->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_game->window.close();
			break;
		case sf::Event::Resized:
			m_gameView.setSize(event.size.width, event.size.height);
			m_guiView.setSize(event.size.width, event.size.height);
			m_game->background.setPosition(m_game->window.mapPixelToCoords(sf::Vector2i(0, 0), m_guiView));
			m_game->background.setScale(
				float(event.size.width) / float(m_game->background.getTexture()->getSize().x),
				float(event.size.height) / float(m_game->background.getTexture()->getSize().y));
			break;
		case sf::Event::MouseMoved:
			// Pan the camera
			if (actionState == ActionState::PANNING)
			{
				sf::Vector2f pos{ sf::Vector2f(sf::Mouse::getPosition(m_game->window) - panningAnchor) };
				m_gameView.move(-1.f * pos * zoomLevel);
				panningAnchor = sf::Mouse::getPosition(m_game->window);
			}
			break;
		case sf::Event::MouseButtonPressed:
			// start panning
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				if (actionState != ActionState::PANNING)
				{
					actionState = ActionState::PANNING;
					panningAnchor = sf::Mouse::getPosition(m_game->window);
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				actionState = ActionState::NONE;
			}
			break;
			// Zoom the view
		case sf::Event::MouseWheelMoved:
			if (event.mouseWheel.delta < 0)
			{
				m_gameView.zoom(2.f);
				zoomLevel *= 2.f;
			}
			else
			{
				m_gameView.zoom(0.5f);
				zoomLevel *= 0.5f;
			}
			break;
		default:
			break;
		}
	}
}

GameStateEditor::GameStateEditor(Game* game)
{
	m_game = game;

	sf::Vector2f pos{ sf::Vector2f(m_game->window.getSize()) };
	
	m_guiView.setSize(pos);
	m_gameView.setSize(pos);

	pos *= 0.5f;

	m_guiView.setCenter(pos);
	m_gameView.setCenter(pos);

	map = Map("city_map.dat", 64, 64, game->tileAtlas);

	zoomLevel = 1.0f;

	// Center the camera on the map
	sf::Vector2f center(map.m_width, map.m_height * 0.5f);
	center *= float(map.m_tileSize);
	m_gameView.setCenter(center);

	actionState = ActionState::NONE;
}
