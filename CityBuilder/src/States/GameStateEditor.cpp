#include "GameStateEditor.h"

void GameStateEditor::draw(const float dt)
{
	m_game->window.clear(sf::Color::Black);

	m_game->window.setView(m_guiView);
	m_game->window.draw(m_game->background);

	m_game->window.setView(m_gameView);
	city.map.draw(m_game->window, dt);
}

void GameStateEditor::update(const float dt)
{
	city.update(dt);

	// update the infobar
	
	guiSystem.at("infobar").setEntryText(0, "Day " + std::to_string(city.day));
	guiSystem.at("infobar").setEntryText(1, "&" + std::to_string(long(city.funds)));
	guiSystem.at("infobar").setEntryText(2, std::to_string(long(city.population)) +
		" (" + std::to_string(long(city.getHomeless())) + ")");
	guiSystem.at("infobar").setEntryText(3, std::to_string(long(city.employable)) + " (" +
		std::to_string(long(city.getUnemployed())) + ")");
	guiSystem.at("infobar").setEntryText(4, tileTypeToStr(currentTile->m_tileType));

	// Highlight entries of the right click menu
	guiSystem.at("rightClickMenu").highlight(guiSystem.at("rightClickMenu")
		.getEntry(m_game->window.mapPixelToCoords(sf::Mouse::getPosition(m_game->window), m_guiView)));
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
			m_gameView.setSize(event.size.width, event.size.height);
			m_gameView.zoom(zoomLevel);
			break;
		case sf::Event::MouseMoved:
			// Pan the camera
			if (actionState == ActionState::PANNING)
			{
				sf::Vector2f pos{ sf::Vector2f(sf::Mouse::getPosition(m_game->window) - panningAnchor) };
				m_gameView.move(-1.f * pos * zoomLevel);
				panningAnchor = sf::Mouse::getPosition(m_game->window);
			}
			else if (actionState == ActionState::SELECTING)
			{
				sf::Vector2f pos{m_game->window.mapPixelToCoords(sf::Mouse::getPosition(m_game->window), m_gameView) };
				
				selectionEnd.x = pos.y / (city.map.m_tileSize) + pos.x / (2 * city.map.m_tileSize) - city.map.m_width * 0.5f - 0.5f;
				selectionEnd.y = pos.y / (city.map.m_tileSize) - pos.x / (2 * city.map.m_tileSize) + city.map.m_width * 0.5f + 0.5f;

				city.map.clearSelected();

				if (currentTile->m_tileType == TileType::GRASS)
				{
					city.map.select(selectionStart, selectionEnd, { currentTile->m_tileType, TileType::WATER });
				}
				else
				{
					city.map.select(selectionStart, selectionEnd,
						{
							currentTile->m_tileType, TileType::FOREST,
							TileType::WATER, TileType::ROAD,
							TileType::RESIDENTIAL, TileType::COMMERCIAL,
							TileType::INDUSTRIAL
						});
				}
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
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				// select a map tile
				if (actionState != ActionState::SELECTING)
				{
					actionState = ActionState::SELECTING;
					sf::Vector2f pos{ m_game->window.mapPixelToCoords(sf::Mouse::getPosition(m_game->window), m_gameView) };

					selectionStart.x = pos.y / (city.map.m_tileSize) + pos.x / (2 * city.map.m_tileSize) - city.map.m_width * 0.5f - 0.5f;

					selectionStart.y = pos.y / (city.map.m_tileSize) - pos.x / (2 * city.map.m_tileSize) + city.map.m_width * 0.5f + 0.5f;
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				// stop selecting
				if (actionState == ActionState::SELECTING)
				{
					actionState = ActionState::NONE;
					city.map.clearSelected();
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				actionState = ActionState::NONE;
			}
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (actionState == ActionState::SELECTING)
				{
					actionState = ActionState::NONE;
					city.map.clearSelected();
				}
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

	//map = Map("city_map.dat", 64, 64, game->tileAtlas);
	city = City("city", game->tileSize, game->tileAtlas);
	city.shuffleTiles();

	guiSystem.emplace("rightClickMenu",
		GUI(sf::Vector2f(196, 16), 2, false, game->m_styleSheets.at("button"),
			{
					std::make_pair("Flatten $" + game->tileAtlas["grass"].getCost(), "grass"),
					std::make_pair("Forest $" + game->tileAtlas["forest"].getCost(), "forest"),
					std::make_pair("Residential zone $" + game->tileAtlas["residential"].getCost(), "residential"),
					std::make_pair("Commercial zone $" + game->tileAtlas["commercial"].getCost(), "commercial"),
					std::make_pair("Industrial zone $" + game->tileAtlas["industrial"].getCost(), "industrial"),
					std::make_pair("Road $" + game->tileAtlas["road"].getCost(), "road")

			}));

	guiSystem.emplace("selectionCostText",
		GUI(sf::Vector2f(196, 16), 0, false, game->m_styleSheets.at("text"), { std::make_pair("","") }));

	guiSystem.emplace("infoBar",
		GUI(sf::Vector2f(game->window.getSize().x / 5, 16), 2, true, game->m_styleSheets.at("button"),
			{
				std::make_pair("time", "time"),
				std::make_pair("funds", "funds"),
				std::make_pair("population", "population"),
				std::make_pair("employment", "employment"),
				std::make_pair("current tile", "tile")
			}));

	guiSystem.at("infoBar").setPosition(sf::Vector2f(0, game->window.getSize().y - 16));
	guiSystem.at("infobar").show();

	zoomLevel = 1.0f;

	// Center the camera on the map
	sf::Vector2f center(city.map.m_width, city.map.m_height * 0.5f);
	center *= float(city.map.m_tileSize);
	m_gameView.setCenter(center);

	selectionStart = sf::Vector2i(0, 0);
	selectionEnd = sf::Vector2i(0, 0);

	currentTile = &game->tileAtlas.at("grass");

	actionState = ActionState::NONE;
}
