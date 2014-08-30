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
}

void TileDestroyer::Update()
{
	auto scale = m_GameObject->Transform()->Scale();
	auto newScale = Lerp(scale, sf::Vector2f(0, 0), GameTime::DeltaTime() * ConfigManager::GetFloat("[Destroyer]fDestroyTileSpeed"));
	if (Length(newScale - sf::Vector2f(0, 0)) < 0.01)
	{
		GameObject::Destroy(m_GameObject);
	}
	else
	{
		m_GameObject->Transform()->SetScale(newScale);
	}
}