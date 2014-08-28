#include "TargetPower.h"
#include "Prefabs.h"
#include "GhostPower.h"
#include "Constants.h"
#include "BoardMap.h"

#include <iostream>
#include <GameObject.h>
#include <assert.h>

TargetPower::TargetPower(sf::Vector2f const& targetPos)
: Power(targetPos), m_GhostPower(nullptr)
{
}

void TargetPower::Awake()
{
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
}

void TargetPower::OnClicked()
{
	if (m_GhostPower != nullptr)
	{
		CancelTarget();
		return;
	}

	auto ghost = prefabs::CreateGhostPower(this);
	m_GhostPower = ghost->GetComponent<GhostPower>();
	PopulateGhostPower();
	GameObject::Instantiate(ghost);
}

void TargetPower::Update()
{
	Power::Update();
}

void TargetPower::CancelTarget()
{
	assert(m_GhostPower != nullptr);

	GameObject::Destroy(m_GhostPower->GetGameObject());
	m_GhostPower = nullptr;
}

void TargetPower::OnPowerPlaced(sf::Vector2i const& pos)
{
	UsePower(pos);
	
	CancelTarget();
	GameObject::Destroy(m_GameObject);
}