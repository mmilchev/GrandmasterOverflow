#include "TileDestroyer.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <Utils.h>
#include <ConfigManager.h>
#include <GameTime.h>
#include <SpriteRenderer.h>

void TileDestroyer::Awake()
{
	m_GameObject->GetComponent<SpriteRenderer>()->SetSpriteColor(sf::Color::Red);
	m_ScaleTween.Set(1, 0, ConfigManager::GetFloat("[Destroyer]fDestroyTileTime"), Tween::EaseOutCubic);
}

void TileDestroyer::Update()
{
	m_ScaleTween.Update(GameTime::DeltaTime());
	if (m_ScaleTween.Done())
	{
		GameObject::Destroy(m_GameObject);
	}
	else
	{
		auto scaleX = m_ScaleTween.GetValue();
		m_GameObject->Transform()->SetScale(sf::Vector2f(scaleX, scaleX));
	}
}