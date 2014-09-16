#ifndef SCREEN_POSITION_ANIMATION_H
#define SCREEN_POSITION_ANIMATION_H

#include <DynamicBehaviour.h>
#include <Tween2.h>

class ScreenPositionAnimation
	: public DynamicBehaviour
{
public:
	ScreenPositionAnimation(sf::Vector2f const& startPos, sf::Vector2f const& endPos);

	void Update() override;
private:
	Tween2 m_PosTween;
};

#endif