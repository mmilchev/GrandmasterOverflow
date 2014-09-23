#ifndef SCREEN_POSITION_ANIMATION_H
#define SCREEN_POSITION_ANIMATION_H

#include <DynamicBehaviour.h>
#include <Tween2.h>
#include <functional>

class ScreenPositionAnimation
	: public DynamicBehaviour
{
public:
	ScreenPositionAnimation(sf::Vector2f const& startPos, sf::Vector2f const& endPos);

	void SetOnAnimationFinishedAction(std::function<void()> action);

	void Update() override;

	void OnClicked() override;

private:
	bool m_Clicked;
	
	Tween2 m_EaseOutPosition;
	Tween2 m_EaseInPosition;

	std::function<void()> m_OnAnimationFinished;
};

#endif