#include "FlowTile.h"
#include "BoardMap.h"
#include "Constants.h"

#include <GameObject.h>
#include <ConfigManager.h>
#include "Prefabs.h"
#include <Utils.h>
#include <GameTime.h>
#include <iostream>

FlowTile::FlowTile(int group)
:m_ShouldSpread(true), m_Group(group), 
m_GameState(nullptr), m_Board(nullptr)
{
}

void FlowTile::OnDestruction()
{
	m_GameState->ReportFlowTileDestroyed(this);
}

void FlowTile::Start()
{
	m_GameState = GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>();
	m_GameState->ReportFlowTileCreated(this);
	
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	
	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());
	m_Board->SetOccupation(this, boardPos);

	float scale = ConfigManager::GetFloat("[Flow Graphics]fStartSizeCoef");
	m_GameObject->Transform()->SetScale(sf::Vector2f(scale, scale));

	ResetTurns();
}

void FlowTile::Update()
{
	auto scale = m_GameObject->Transform()->Scale();
	m_GameObject->Transform()->SetScale(Lerp(scale, sf::Vector2f(1, 1), GameTime::DeltaTime() * ConfigManager::GetFloat("[Flow Graphics]fGrowSpeed")));
}

void FlowTile::OnTurnTime()
{
	m_TurnsLeft--; 
	if (m_ShouldSpread && m_TurnsLeft <= 0)
	{
		Spread();
		ResetTurns();
		m_ShouldSpread = true;
	}
}

void FlowTile::DestroySurroundings()
{
	const sf::Vector2i dirs[] =
	{
		sf::Vector2i(1, 0),
		sf::Vector2i(-1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(0, -1)
	};

	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());

	for (auto const& dir : dirs)
	{
		auto pos = boardPos + dir;
		auto state = m_Board->GetTileState(pos);
		if (state != nullptr
			&& state->GetTileType() == TileType::Solid)
		{
			m_Board->CreateEmptyTile(pos);
			GameObject::Instantiate(prefabs::CreateTileDestroyer(m_Board->GetWorldPos(pos)));
		}
	}
}

void FlowTile::ResetTurns()
{
	m_TurnsLeft = ConfigManager::GetInt("[Flow Gameplay]iSpreadTurnTime");
}

void FlowTile::Spread()
{
	const sf::Vector2i dirs[] =
	{
		sf::Vector2i(1, 0),
		sf::Vector2i(-1, 0),
		sf::Vector2i(0, 1),
		sf::Vector2i(0, -1)
	};

	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());

	for (auto const& dir : dirs)
	{
		auto pos = boardPos + dir;
		auto state = m_Board->GetTileState(pos);
		if (state != nullptr)
		{
			if (state->IsPassable() && !state->Occupied())
			{
				auto gObject = prefabs::CreateFlow(m_Board->GetWorldPos(pos), m_Group);
				GameObject::Instantiate(gObject);
				m_Board->SetOccupation(gObject->GetComponent<FlowTile>(), pos);
				m_GameState->ReportTileSpread(this);
			}
			else if (state->Occupied() && state->GetOccupant()->GetFlowGroup() != m_Group)
			{
				m_GameState->MergeFlowGroups(state->GetOccupant()->GetFlowGroup(), m_Group);
			}
		}
	}
}