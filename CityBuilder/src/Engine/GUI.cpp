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
		text.setFillColor(style.m_textCol);
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
	if (entry >= m_entries.size() || entry < 0) return;
	
	m_entries[entry].m_text.setString(text);
}

void GUI::setDimensions(sf::Vector2f dimensions)
{
	m_dimensions = dimensions;

	for (auto& entry : m_entries)
	{
		entry.m_shape.setSize(m_dimensions);
		entry.m_text.setCharacterSize(dimensions.y - m_style.m_borderSize - m_padding);
	}
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!m_visible) return;

	// Draw each entry in the menu
	for (auto entry : m_entries)
	{
		target.draw(entry.m_shape);
		target.draw(entry.m_text);
	}
}

void GUI::show()
{
	sf::Vector2f offset(0.f, 0.f);

	m_visible = true;

	for (auto& entry : m_entries)
	{
		sf::Vector2f origin{ getOrigin() };
		origin -= offset;
		entry.m_shape.setOrigin(origin);
		entry.m_text.setOrigin(origin);

		entry.m_shape.setPosition(getPosition());
		entry.m_text.setPosition(getPosition());

		if (m_horizontal) offset.x += m_dimensions.x;

		else offset.y += m_dimensions.y;
	}
}

void GUI::hide()
{
	m_visible = false;
}

void GUI::highlight(const int entry)
{
	for (int i{ 0 }; i < m_entries.size(); ++i)
	{
		if (i == entry)
		{
			m_entries[i].m_shape.setFillColor(m_style.m_bodyHighlightCol);
			m_entries[i].m_shape.setOutlineColor(m_style.m_borderHighlightCol);
			m_entries[i].m_text.setFillColor(m_style.m_textHightlightCol);
		}
		else
		{
			m_entries[i].m_shape.setFillColor(m_style.m_bodyCol);
			m_entries[i].m_shape.setOutlineColor(m_style.m_borderCol);
			m_entries[i].m_text.setFillColor(m_style.m_textCol);
		}
	}
}

std::string GUI::activate(const int entry)
{
	if (entry == -1) return "null";
	return m_entries[entry].m_message;
}

std::string GUI::activate(const sf::Vector2f mousePos)
{
	int entry{ getEntry(mousePos) };
	return activate(entry);
}
