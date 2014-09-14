#include "PauseIndicator.h"
#include "ConfigManager.h"
#include "GameTime.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <SpriteRenderer.h>

void PauseIndicator::Awake()
{
	m_FadeTween.Set(0, 1, ConfigManager::GetFloat("[GUI]fPauseIconTime"), Tween::Logaritmic);
	m_Renderer = m_GameObject->GetComponent<SpriteRenderer>();
	
	//Pause the game time
	GameTime::SetScale(0);
}

void PauseIndicator::Update()
{
	m_FadeTween.Update(GameTime::DeltaTimeUnscaled());

	if (!m_FadeTween.Done())
	{
		auto scaleFactor = 1 + m_FadeTween.GetValue() * ConfigManager::GetFloat("[GUI]fPauseIconScale");
		m_GameObject->Transform()->SetScale(sf::Vector2f(scaleFactor, scaleFactor));
		auto color = m_Renderer->GetSprite().getColor();
		color.a = static_cast<sf::Uint8>((1 - m_FadeTween.GetValue()) * 255);
		m_Renderer->SetSpriteColor(color);
	}
}

void PauseIndicator::OnClicked()
{
	GameObject::Destroy(m_GameObject);
	
	//Resume game time
	GameTime::SetScale(1);
}
