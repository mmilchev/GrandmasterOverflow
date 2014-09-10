#include "GhostPower.h"
#include "TargetPower.h"
#include "Constants.h"
#include "BoardMap.h"

#include <GameObject.h>
#include <Input.h>
#include <CameraComponent.h>


GhostPower::GhostPower(TargetPower* power)
	:m_Power(power), m_Origin(0, 0)
{
}

void GhostPower::Awake()
{
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
}

void GhostPower::Update()
{
	auto mPos = m_Origin + GameObject::FindByTag(TAG_GAME_CAMERA)->GetComponent<CameraComponent>()->ScreenToWorldSpace(Input::GetMousePos());
	auto boardPos = m_Board->GetGridPos(mPos);

	if (m_Board->IsInBounds(boardPos))
	{
		m_GameObject->Transform()->SetPosition(-m_Origin + m_Board->GetWorldPos(boardPos));

		if (Input::GetButtonDown(sf::Mouse::Button::Left) &&
			m_Power->CanBePlaced(boardPos))
		{
			m_Power->OnPowerPlaced(boardPos);
		}
	}

	if (Input::GetKeyDown(sf::Keyboard::Q)
		|| Input::GetButtonDown(sf::Mouse::Button::Right))
	{
		m_GameObject->Transform()->SetRotation(m_GameObject->Transform()->Rotation() + 90);
	}
	else if (Input::GetKeyDown(sf::Keyboard::E))
	{
		m_GameObject->Transform()->SetRotation(m_GameObject->Transform()->Rotation() - 90);
	}

	if (Input::GetKeyDown(sf::Keyboard::Key::Escape)
		|| Input::GetButtonDown(sf::Mouse::Button::Middle))
	{
		m_Power->CancelTarget();
	}
}