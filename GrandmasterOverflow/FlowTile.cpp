#include "FlowTile.h"
#include "BoardMap.h"
#include "Constants.h"

#include <GameObject.h>
#include <ConfigManager.h>
#include "Prefabs.h"
#include <Utils.h>
#include <GameTime.h>
#include <iostream>
#include <SpriteRenderer.h>

std::map<FlowTile::FlowTileType, sf::Color> FlowTile::sTileColours =
{
	{ Red, sf::Color(255, 2, 2) },
	{ Blue, sf::Color(0, 65, 225) },
	{ Teal, sf::Color(27, 225, 184) },
	{ Yellow, sf::Color(255, 225, 0) },
	{ Orange, sf::Color(254, 137, 13) },
	{ Green, sf::Color(31, 191, 0) },
	{ Pink, sf::Color(255, 90, 170) },
	{ Gray, sf::Color(148, 149, 150) },
	{ LightBlue, sf::Color(125, 190, 241) },
};

FlowTile::FlowTile(FlowTileType type, int group)
:m_Group(group), kType(type), m_ShouldSpread(true),
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

	m_GameObject->GetComponent<SpriteRenderer>()->SetSpriteColor(sTileColours[kType]);

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

void FlowTile::Solidify()
{
	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());
	m_Board->CreateSolidTile(boardPos);
	GameObject::Destroy(m_GameObject);
	GameObject::Instantiate(prefabs::CreateTileDestroyer(m_GameObject->Transform()->Position()));
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
				auto gObject = prefabs::CreateFlow(m_Board->GetWorldPos(pos), kType, m_Group);
				GameObject::Instantiate(gObject);
				m_Board->SetOccupation(gObject->GetComponent<FlowTile>(), pos);
				m_GameState->ReportTileSpread(this);
			}
			else if (state->Occupied() && state->GetOccupant()->GetFlowGroup() != m_Group
				&& state->GetOccupant()->kType == kType)
			{
				m_GameState->MergeFlowGroups(state->GetOccupant()->GetFlowGroup(), m_Group);
			}
		}
	}
}