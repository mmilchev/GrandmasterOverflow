#include "ScreenPositionAnimation.h"
#include <ConfigManager.h>
#include <GameTime.h>
#include <GameObject.h>
#include <TransformComponent.h>

ScreenPositionAnimation::ScreenPositionAnimation(sf::Vector2f const& startPos, sf::Vector2f const& endPos)
{
	m_PosTween.Set(startPos, endPos, ConfigManager::GetFloat("[GUI]fPositionAnimationTime"), Tween::TweenType::Expo5);
}

void ScreenPositionAnimation::Update()
{
	m_PosTween.Update(GameTime::DeltaTimeUnscaled());
	if (m_PosTween.Done())
		GameObject::Destroy(m_GameObject);

	m_GameObject->Transform()->SetPosition(m_PosTween.GetValue());
}
