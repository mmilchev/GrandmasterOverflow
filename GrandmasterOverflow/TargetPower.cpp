#include "TargetPower.h"
#include "Prefabs.h"
#include "GhostPower.h"
#include "Constants.h"
#include "BoardMap.h"
#include "GameState.h"

#include <iostream>
#include <GameObject.h>
#include <assert.h>

TargetPower::TargetPower( int uses ) 
	: Power(uses), m_GhostPower(nullptr), m_GameState(nullptr)
{
}

void TargetPower::Awake()
{
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	m_GameState = GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>();
	m_GameState->ReportPowerCreated(this);
}

void TargetPower::OnClicked()
{
	TriggerPower();
}

void TargetPower::TriggerPower()
{
	m_GameState->OnPowerSelected(this);

	if (m_GhostPower != nullptr)
	{
		CancelTarget();
		return;
	}

	if (CanUsePower())
	{
		auto ghost = prefabs::CreateGhostPower(this);
		m_GhostPower = ghost->GetComponent<GhostPower>();
		PopulateGhostPower();
		GameObject::Instantiate(ghost);
	}
}

void TargetPower::CancelTarget()
{
	if (m_GhostPower != nullptr)
	{
		GameObject::Destroy(m_GhostPower->GetGameObject());
		m_GhostPower = nullptr;
	}
}

void TargetPower::OnPowerPlaced(sf::Vector2i const& pos)
{
	UsePower(pos);
	
	CancelTarget();
	OnPowerUsed();

	TriggerPower();
}
