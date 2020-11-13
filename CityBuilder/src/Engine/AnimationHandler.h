#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
public:
	unsigned int m_startFrame;
	unsigned int m_endFrame;

	float m_duration;

	Animation(unsigned int startFrame, unsigned int endFrame, float duration)
		: m_startFrame(startFrame), m_endFrame(endFrame), m_duration(duration)
	{ }

	unsigned int getLength()
	{
		return m_endFrame - m_startFrame + 1;
	}
};

class AnimationHandler
{

};