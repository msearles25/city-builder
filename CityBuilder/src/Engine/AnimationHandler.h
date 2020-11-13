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
private:
	std::vector<Animation> m_animations;
	float m_t;
	int m_currentAnim;
public:
	// Add an animation
	void addAnim(Animation& anim);

	/*
	Update the current frame of animation. dt is the time since the update was last called
	(The time for one frame to be executed)
	*/
	void update(const float dt);

	// Change the animation, resetting it in the process
	void changeAnim(unsigned int animID);

	// Current section that should be displayed
	sf::IntRect m_bounds;

	// Pixel dimensions of each frame
	sf::IntRect m_frameSize;

	AnimationHandler();
	AnimationHandler(const sf::IntRect& frameSize);
};