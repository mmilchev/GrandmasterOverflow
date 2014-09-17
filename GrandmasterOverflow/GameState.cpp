#include "GameState.h"
#include "BoardMap.h"
#include "Constants.h"
#include "ITurnClient.h"
#include "FlowTile.h"
#include "TargetPower.h"
#include "Prefabs.h"
#include "ButtonBehaviour.h"
#include "BoardMap.h"

#include <SFML/System/Vector2.hpp>
#include <GameObject.h>
#include <CameraComponent.h>
#include <GameTime.h>
#include <ConfigManager.h>
#include <SFML/Window.hpp>
#include <SpriteRenderer.h>
#include <Application.h>

GameState::GameState()
	:m_TurnNum(0), m_GameOver(false)
{
}

void GameState::Start()
{
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();

	SetTurnTime(ConfigManager::GetFloat("[Gameplay]fTurnTime"));
}

void GameState::Update()
{
	if (m_GameOver)
		return;

	m_TimeToNextTurn -= GameTime::DeltaTime();
	if (m_TimeToNextTurn <= 0)
	{
		m_TurnNum++;
		m_TimeToNextTurn += m_TurnTime;

		m_FlowTiles.ProcessQueued();

		CheckAndSolidifyTiles();
		ExecuteTurn();

		if (m_TurnNum % ConfigManager::GetInt("[Flow Gameplay]iSpreadTurnTime") == 0)
		{
			std::vector<int> groups;
			GetTileGroups(groups);

			if (groups.size() == 0)
			{
				TriggerGameOver();
				return;
			}

			std::sort(groups.begin(), groups.end());
			std::sort(m_GroupMovedThisTurn.begin(), m_GroupMovedThisTurn.end());

			std::vector<int> diff;
			std::set_difference(groups.begin(), groups.end(), m_GroupMovedThisTurn.begin(), m_GroupMovedThisTurn.end(), std::back_inserter(diff));

			for (unsigned int i = 0; i < diff.size(); i++)
			{
				SolidifyTileGroup(diff[i]);
			}
			m_FlowTiles.ProcessQueued();

			m_GroupMovedThisTurn.clear();
		}
	}
}

void GameState::SetTurnTime(float turnTime)
{
	m_TurnTime = turnTime;
	m_TimeToNextTurn = m_TurnTime;
}

void GameState::ReportFlowTileCreated(FlowTile* tile)
{
	m_FlowTiles.Add(tile);
}

void GameState::ReportFlowTileDestroyed(FlowTile* tile)
{
	m_FlowTiles.Remove(tile);
}

void GameState::ReportTileActivity(FlowTile* tile)
{
	if (std::find(m_GroupMovedThisTurn.begin(), m_GroupMovedThisTurn.end(), tile->GetFlowGroup()) == m_GroupMovedThisTurn.end())
		m_GroupMovedThisTurn.push_back(tile->GetFlowGroup());
}

void GameState::GetTileGroups(std::vector<int>& groups)
{
	int group;
	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		group = m_FlowTiles[i]->GetFlowGroup();
		if (std::find(groups.begin(), groups.end(), group) == groups.end())
			groups.push_back(group);
	}
}

void GameState::SolidifyTileGroup(int group)
{
	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		auto tile = m_FlowTiles[i];
		if (tile->GetFlowGroup() == group)
			tile->Solidify();
	}
}

void GameState::TriggerGameOver()
{
	GameObject::Instantiate(prefabs::CreateLevelCompleteAnimation(static_cast<int>(m_Board->GetPercentFilled())));
	m_GameOver = true;
}

void GameState::CheckAndSolidifyTiles()
{
	std::vector<int> groupsForSolidifying;

	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		auto tile = m_FlowTiles[i];
		if (tile->CheckCollision() != nullptr || tile->IsOutOfMoves())
		{
			groupsForSolidifying.push_back(tile->GetFlowGroup());
		}
	}

	for (auto group : groupsForSolidifying)
	{
		SolidifyTileGroup(group);
	}

	m_FlowTiles.ProcessQueued();
}

void GameState::ExecuteTurn()
{
	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		m_FlowTiles[i]->OnTurnTime();
	}

	m_FlowTiles.ProcessQueued();
}

void GameState::ReportPowerCreated(TargetPower* power)
{
	m_TargetPowers.push_back(power);
}

void GameState::OnPowerSelected(TargetPower* power)
{
	for (auto innerPower : m_TargetPowers)
	{
		if (innerPower != power)
			innerPower->CancelTarget();
	}
}
