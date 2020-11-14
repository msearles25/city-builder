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

class GUI
{

};