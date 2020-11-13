#include "AnimationHandler.h"

void AnimationHandler::addAnim(Animation& anim)
{
	m_animations.push_back(anim);
}

void AnimationHandler::update(const float dt)
{
	if (m_currentAnim >= m_animations.size() || m_currentAnim < 0) return;

	float duration{ m_animations[m_currentAnim].m_duration };

	// check if the animation has progressed to a new frame, if so then change to the next frame
	if (int((m_t + dt) / duration) > int(m_t / duration))
	{
		// Calculate the frame number
		int frame{ int((m_t + dt) / duration) };

		// Adjust for looping
		frame %= m_animations[m_currentAnim].getLength();

		// Set the sprite to the frame
		sf::IntRect rect{ m_frameSize };
		rect.left = rect.width * frame;
		rect.top = rect.height * m_currentAnim;
		m_bounds = rect;
	}

	// Increment time elapsed
	m_t += dt;

	// adjusting for loop
	if (m_t > duration * m_animations[m_currentAnim].getLength())
	{
		m_t = 0.f;
	}
}

void AnimationHandler::changeAnim(unsigned int animID)
{
	// Do not change the animation if it is currently active or the new anim doesnt exist
	if (m_currentAnim == animID || animID >= m_animations.size())
		return;

	// Set current anim
	m_currentAnim = animID;
	// update the anims bounds
	sf::IntRect rect{ m_frameSize };
	rect.top = rect.height * animID;
	m_bounds = rect;
	m_t = 0.f;
}

AnimationHandler::AnimationHandler()
{
	m_t = 0.f;
	m_currentAnim = -1;
}

AnimationHandler::AnimationHandler(const sf::IntRect& frameSize)
{
	m_frameSize = frameSize;
	m_t = 0.f;
	m_currentAnim = -1;
}