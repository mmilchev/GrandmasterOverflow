#include "ScreenIconAnimation.h"
#include "ConfigManager.h"
#include "GameTime.h"
#include "Utils.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <SpriteRenderer.h>

void ScreenIconAnimation::Awake()
{
	m_FadeTween.Set(0, 1, ConfigManager::GetFloat("[GUI]fPauseIconTime"), Tween::Expo3);
	m_Renderer = m_GameObject->GetComponent<SpriteRenderer>();
}

void ScreenIconAnimation::Update()
{
	m_FadeTween.Update(GameTime::DeltaTimeUnscaled());

	if (!m_FadeTween.Done())
	{
		auto value = m_FadeTween.GetValue();
		auto scaleValue = Clamp(value, 0, 1);
		m_GameObject->Transform()->SetScale(sf::Vector2f(scaleValue, scaleValue));

		auto color = m_Renderer->GetSprite().getColor();
		float alpha = Clamp(2 - 2 * value, 0, 1);
		color.a = static_cast<sf::Uint8>(alpha  * 255);

		m_Renderer->SetSpriteColor(color);
	}
	else
	{
		GameObject::Destroy(m_GameObject);
	}
}
