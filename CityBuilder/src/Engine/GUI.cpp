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

sf::Vector2f GUI::getSize()
{
	return sf::Vector2f(m_dimensions.x, m_dimensions.y * m_entries.size());
}

int GUI::getEntry(const sf::Vector2f mousePos)
{
	if (m_entries.size() == 0) return -1;
	if (!m_visible) return -1;

	for (int i{ 0 }; i < m_entries.size(); ++i)
	{
		// Translate point to use the entries local coords
		sf::Vector2f point{ mousePos };
		point += m_entries[i].m_shape.getOrigin();
		point -= m_entries[i].m_shape.getPosition();

		if (point.x < 0 || point.x > m_entries[i].m_shape.getScale().x * m_dimensions.x) continue;
		if (point.y < 0 || point.y > m_entries[i].m_shape.getScale().y * m_dimensions.y) continue;

		return i;
	}

	return -1;
}

void GUI::setEntryText(int entry, std::string text)
{
}

void GUI::setDimensions(sf::Vector2f dimensions)
{
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void GUI::show()
{
}

void GUI::hide()
{
}

void GUI::highlight(const int entry)
{
}

std::string GUI::activate(const int entry)
{
	return std::string();
}

std::string GUI::activate(const sf::Vector2f mousePos)
{
	return std::string();
}
