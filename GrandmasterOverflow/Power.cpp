#include "Power.h"
#include "Constants.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <GameTime.h>
#include <SpriteRenderer.h>
#include <TextRenderer.h>

#include <iostream>

Power::Power(int uses)
	:m_Uses(uses)
{
	m_ScaleTween.Set(1, 1, 1, Tween::TweenType::Logaritmic);
}

void Power::OnMouseEnter()
{
	m_ScaleTween.Set(m_GameObject->Transform()->Scale().x, 1.3f, 0.3f, Tween::TweenType::Logaritmic);
}

void Power::OnMouseLeave()
{
	m_ScaleTween.Set(m_GameObject->Transform()->Scale().x, 1, 0.1f, Tween::TweenType::Logaritmic);
}

void Power::Update()
{
	if (!m_ScaleTween.Done())
	{
		m_ScaleTween.Update(GameTime::DeltaTimeUnscaled());
		auto value = m_ScaleTween.GetValue();
		m_GameObject->Transform()->SetScale(sf::Vector2f(value, value));
	}
}

void Power::OnPowerUsed()
{
	m_Uses--;
	m_GameObject->GetChildWithName(NAME_TEXT_POWER)->GetComponent<TextRenderer>()->Text().setString("x" + std::to_string(m_Uses));
}

bool Power::CanUsePower()
{
	return m_Uses > 0;
}
