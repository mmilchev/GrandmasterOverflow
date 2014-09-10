#include "FlowTile.h"
#include "BoardMap.h"
#include "Constants.h"
#include "Prefabs.h"

#include <GameObject.h>
#include <ConfigManager.h>
#include <Utils.h>
#include <GameTime.h>
#include <iostream>
#include <SpriteRenderer.h>
#include <TextRenderer.h>

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

std::vector<sf::Vector2i> const FlowTile::skSpreadDirections = {
	sf::Vector2i(1, 0),
	sf::Vector2i(-1, 0),
	sf::Vector2i(0, 1),
	sf::Vector2i(0, -1)
};

FlowTile::FlowTile(FlowTileType type, int group, int turns) :m_Group(group), kType(type), m_ShouldSpread(true),
m_GameState(nullptr), m_Board(nullptr), m_SpreadTurnsLeft(turns)
{
}

void FlowTile::OnDestruction()
{
	m_GameState->ReportFlowTileDestroyed(this);

	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());
	m_Board->CreateSolidTile(boardPos);
	GameObject::Instantiate(prefabs::CreateTileDestroyer(m_GameObject->Transform()->Position()));
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
	sf::Vector2f scaleVec = sf::Vector2f(scale, scale);
	m_GameObject->Transform()->SetScale(scaleVec);

	m_ScaleTween.Set(scaleVec, sf::Vector2f(1, 1), ConfigManager::GetFloat("[Flow Graphics]fGrowSpeed"), Tween::TweenType::Logaritmic);

	ResetTurns();
}

void FlowTile::Update()
{
	if (!m_ScaleTween.Done())
	{
		m_ScaleTween.Update(GameTime::DeltaTime());
		m_GameObject->Transform()->SetScale(m_ScaleTween.GetValue());
	}
}

void FlowTile::OnTurnTime()
{
	m_TurnsLeft--;
	if (m_ShouldSpread && m_TurnsLeft <= 0)
	{
		Spread();
		ResetTurns();
		m_ShouldSpread = true;

		if (m_SpreadTurnsLeft == 0)
		{
			m_GameState->SolidifyTileGroup(m_Group);
		}
	}
}

void FlowTile::Solidify()
{
	GameObject::Destroy(m_GameObject);
}

void FlowTile::ResetTurns()
{
	m_TurnsLeft = ConfigManager::GetInt("[Flow Gameplay]iSpreadTurnTime");
}

void FlowTile::Spread()
{
	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());
	for (auto const& dir : skSpreadDirections)
	{
		auto pos = boardPos + dir;
		auto state = m_Board->GetTileState(pos);
		if (state != nullptr)
		{
			if (state->IsPassable() && !state->Occupied())
			{
				auto gObject = prefabs::CreateFlow(m_Board->GetWorldPos(pos), kType, m_Group, -1);
				GameObject::Instantiate(gObject);
				m_Board->SetOccupation(gObject->GetComponent<FlowTile>(), pos);
				m_GameState->ReportTileActivity(this);
			}
		}
	}

	if (m_SpreadTurnsLeft > 0)
		m_SpreadTurnsLeft--;
	if (m_SpreadTurnsLeft != -1)
	{
		m_GameObject->GetComponent<TextRenderer>()->Text().setString(std::to_string(m_SpreadTurnsLeft));
	}
}

bool FlowTile::CheckCollision()
{
	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());
	std::vector<sf::Vector2i> spawns;

	for (auto const& dir : skSpreadDirections)
	{
		auto pos = boardPos + dir;
		auto state = m_Board->GetTileState(pos);
		if (state != nullptr)
		{
			if (state->IsPassable() && state->Occupied() && 
				state->GetOccupant()->kType != kType && state->GetOccupant()->m_Group != m_Group)
				return true;
		}
	}

	return false;
}
