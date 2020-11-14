#pragma once

#include <string>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>

class GuiStyle
{
public:
	sf::Color m_bodyCol;
	sf::Color m_bodyHighlightCol;
	sf::Color m_borderCol;
	sf::Color m_borderHighlightCol;
	sf::Color m_textCol;
	sf::Color m_textHightlightCol;
	sf::Font* m_font;

	float m_borderSize;

	GuiStyle(sf::Font* font, float bordersize,
		sf::Color bodyCol, sf::Color borderCol, sf::Color textCol,
		sf::Color bodyHighlightCol, sf::Color borderHighlightCol, sf::Color textHighlightCol)
	{
		m_bodyCol = bodyCol;
		m_borderCol = borderCol;
		m_textCol = textCol;
		m_bodyHighlightCol = bodyHighlightCol;
		m_borderHighlightCol = borderHighlightCol;
		m_textHightlightCol = textHighlightCol;
		m_font = font;
		m_borderSize = bordersize;
	}

	GuiStyle() { }
};

class GuiEntry
{
public:
	sf::RectangleShape m_shape;
	std::string m_message;
	sf::Text m_text;

	GuiEntry(const std::string& message, sf::RectangleShape shape, sf::Text text)
	{
		m_message = message;
		m_shape = shape;
		m_text = text;
	}

	GuiEntry() { }
};

class GUI : public sf::Transformable, public sf::Drawable
{
private:
	// if true, the entries will be horizontally adjacent
	bool m_horizontal;

	GuiStyle m_style;
	
	sf::Vector2f m_dimensions;
	int m_padding;
public:
	std::vector<GuiEntry> m_entries;
	bool m_visible;

	GUI(sf::Vector2f dimensions, int padding, bool horizontal,
		GuiStyle& style, std::vector<std::pair<std::string, std::string>> entries);

	sf::Vector2f getSize();

	/*
	Return the entry that the mouse is hovering over. 
	-1 is returned if the mouse is outside of the gui
	*/
	int getEntry(const sf::Vector2f mousePos);

	// Change the text of an entry
	void setEntryText(int entry, std::string text);

	void setDimensions(sf::Vector2f dimensions);

	// Draw the menu
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void show();
	void hide();

	// Highlights an entry of the menu
	void highlight(const int entry);

	// Return the message that's bound to the entry
	std::string activate(const int entry);
	std::string activate(const sf::Vector2f mousePos);
};