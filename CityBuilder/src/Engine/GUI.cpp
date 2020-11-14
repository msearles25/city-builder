#include "GUI.h"

GUI::GUI(sf::Vector2f dimensions, int padding, bool horizontal, GuiStyle& style, std::vector<std::pair<std::string, std::string>> entries)
{
	m_visible = false;
	m_horizontal = horizontal;
	m_style = style;
	m_dimensions = dimensions;
	m_padding = padding;

	// Construct the background shape
	sf::RectangleShape shape;
	shape.setSize(m_dimensions);
	shape.setFillColor(style.m_bodyCol);
	shape.setOutlineThickness(-style.m_borderSize);
	shape.setOutlineColor(style.m_borderCol);

	// Construct each entry
	for (auto entry : entries)
	{
		sf::Text text;
		text.setString(entry.first);
		text.setFont(*style.m_font);
		text.setColor(style.m_textCol);
		text.setCharacterSize(dimensions.y - style.m_borderSize - m_padding);

		m_entries.push_back(GuiEntry(entry.second, shape, text));
	}
}