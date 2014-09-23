#include "ScreenPositionAnimation.h"
#include "LevelManager.h"

#include <ConfigManager.h>
#include <GameTime.h>
#include <GameObject.h>
#include <TransformComponent.h>

ScreenPositionAnimation::ScreenPositionAnimation(sf::Vector2f const& startPos, sf::Vector2f const& endPos)
	:m_Clicked(false), m_OnAnimationFinished([]() {})
{
	auto midPos = startPos + (endPos - startPos) * 0.5f;
	float halfAnimTime = ConfigManager::GetFloat("[GUI]fPositionAnimationTime") / 2;
	//First tween -> Start fast, end slow
	m_EaseOutPosition.Set(startPos, midPos, halfAnimTime, Tween::TweenType::EaseOutCubic);
	//Second tween -> Start slow, end fast
	m_EaseInPosition.Set(midPos, endPos, halfAnimTime, Tween::TweenType::EaseInCubic);
}

void ScreenPositionAnimation::Update()
{
	Tween2* posTween;

	if (m_EaseOutPosition.Done() && m_Clicked)
	{
		m_EaseInPosition.Update(GameTime::DeltaTimeUnscaled());

		if (m_EaseInPosition.Done())
		{
			GameObject::Destroy(m_GameObject);

			m_OnAnimationFinished();
		}
		
		posTween = &m_EaseInPosition;
	}
	else
	{
		m_EaseOutPosition.Update(GameTime::DeltaTimeUnscaled());
		posTween = &m_EaseOutPosition;
	}

	m_GameObject->Transform()->SetPosition(posTween->GetValue());
}

void ScreenPositionAnimation::SetOnAnimationFinishedAction(std::function<void()> action)
{
	m_OnAnimationFinished = action;
}

void ScreenPositionAnimation::OnClicked()
{
	m_Clicked = true;
}
