#include "ButtonBehaviour.h"
#include "GameTime.h"
#include "Prefabs.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <ConfigManager.h>

ButtonBehaviour::ButtonBehaviour()
{
	m_ScaleTween.Set(1, 1, 1, Tween::TweenType::Logaritmic);
}

void ButtonBehaviour::OnMouseEnter()
{
	m_ScaleTween.Set(m_GameObject->Transform()->Scale().x, ConfigManager::GetFloat("[GUI]fButtonExpandScale"), ConfigManager::GetFloat("[GUI]fButtonExpandTime"), Tween::TweenType::Logaritmic);
}

void ButtonBehaviour::OnMouseLeave()
{
	m_ScaleTween.Set(m_GameObject->Transform()->Scale().x, 1, ConfigManager::GetFloat("[GUI]fButtonShrinkTime"), Tween::TweenType::Logaritmic);
}

void ButtonBehaviour::Update()
{
	if (!m_ScaleTween.Done())
	{
		m_ScaleTween.Update(GameTime::DeltaTimeUnscaled());
		auto value = m_ScaleTween.GetValue();
		m_GameObject->Transform()->SetScale(sf::Vector2f(value, value));
	}
}

void ButtonBehaviour::OnClicked()
{
	TriggerButton();
}

void ButtonBehaviour::TriggerButton()
{
}

void PauseButtonBehaviour::TriggerButton()
{
	GameTime::SetScale(0);
	GameObject::Instantiate(prefabs::CreateIconAnimation("pauseIcon.png"));
}

void NormalSpeedButtonBehaviour::TriggerButton()
{
	GameTime::SetScale(1);
	GameObject::Instantiate(prefabs::CreateIconAnimation("normalSpeedIcon.png"));
}

void FastforwardButtonBehaviour::TriggerButton()
{
	GameTime::SetScale(ConfigManager::GetFloat("[Gameplay]fFastForwardSpeed"));
	GameObject::Instantiate(prefabs::CreateIconAnimation("fastForwardIcon.png"));
}
