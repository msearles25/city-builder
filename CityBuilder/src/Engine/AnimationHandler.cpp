#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(const sf::IntRect& frameSize)
	: m_frameSize(frameSize)
{
	m_t = 0.f;
	m_currentAnim = -1;
}