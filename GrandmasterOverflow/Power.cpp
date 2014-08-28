#include "Power.h"
#include "Constants.h"

#include <SFML/Window.hpp>
#include <GameObject.h>
#include <TransformComponent.h>
#include <Utils.h>
#include <GameTime.h>
#include <Input.h>
#include <SpriteRenderer.h>
#include <CameraComponent.h>
#include <iostream>

Power::Power(sf::Vector2f const& targetPos)
:m_TargetPos(targetPos)
{
}

void Power::OnMouseEnter()
{
	auto sRenderer = m_GameObject->GetComponent<SpriteRenderer>();
	sRenderer->SetSpriteColor(sf::Color::Red);
}

void Power::OnMouseLeave()
{
	auto sRenderer = m_GameObject->GetComponent<SpriteRenderer>();
	sRenderer->SetSpriteColor(sf::Color::White);
}

void Power::Update()
{
	m_GameObject->Transform()->SetLocalPosition(Lerp(m_GameObject->Transform()->LocalPosition(), m_TargetPos, GameTime::DeltaTime() * 2));
}