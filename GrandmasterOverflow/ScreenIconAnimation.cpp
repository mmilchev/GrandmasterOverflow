#include "ScreenIconAnimation.h"
#include "ConfigManager.h"
#include "GameTime.h"
#include "Utils.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <SpriteRenderer.h>

void ScreenIconAnimation::Awake()
{
	m_FadeTween.Set(0, 3, ConfigManager::GetFloat("[GUI]fPauseIconTime"), Tween::Linear);
	m_Renderer = m_GameObject->GetComponent<SpriteRenderer>();
}

void ScreenIconAnimation::Update()
{
	m_FadeTween.Update(GameTime::DeltaTimeUnscaled());

	if (!m_FadeTween.Done())
	{
		auto value = m_FadeTween.GetValue();
		auto scaleValue = Clamp(value - 2, 0, 1);
		auto scaleFactor = 1 + (scaleValue)* ConfigManager::GetFloat("[GUI]fPauseIconScale");
		m_GameObject->Transform()->SetScale(sf::Vector2f(scaleFactor, scaleFactor));
		auto color = m_Renderer->GetSprite().getColor();
		
		float fadeValue;
		if (value < 0.5)
			fadeValue = 2*value;
		else if (value > 2)
			fadeValue = 3 - m_FadeTween.GetValue();
		else
			fadeValue = 1;

		color.a = static_cast<sf::Uint8>(fadeValue * 255);
		
		m_Renderer->SetSpriteColor(color);
	}
	else
	{
		GameObject::Destroy(m_GameObject);
	}
}
