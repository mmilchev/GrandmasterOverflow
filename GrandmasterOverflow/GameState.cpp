#include "GameState.h"
#include "BoardMap.h"
#include "Constants.h"
#include "SidebarBahaviour.h"
#include "ITurnClient.h"
#include "FlowTile.h"

#include <SFML/System/Vector2.hpp>
#include <GameObject.h>
#include <CameraComponent.h>
#include <GameTime.h>
#include <ConfigManager.h>
#include <SFML/Window.hpp>
#include <SpriteRenderer.h>
#include <Application.h>

GameState::GameState()
:m_TurnNum(0)
{
}

void GameState::Start()
{	
	SetTurnTime(ConfigManager::GetFloat("[Gameplay]fTurnTime"));
}

void GameState::Update()
{
	m_TimeToNextTurn -= GameTime::DeltaTime();
	if (m_TimeToNextTurn <= 0)
	{
		m_TurnNum++;
		m_TimeToNextTurn += m_TurnTime;
		m_TurnTimeSignal.emit();

		if (m_TurnNum % ConfigManager::GetInt("[Flow Gameplay]iSpreadTurnTime") == 0)
		{
			std::vector<int> groups;
			GetTileGroups(groups);

			if (groups.size() == 0)
			{
				TriggerGameOver();
				GameObject::Destroy(m_GameObject);
				return;
			}

			std::sort(groups.begin(), groups.end());
			std::sort(m_GroupMovedThisTurn.begin(), m_GroupMovedThisTurn.end());

			std::vector<int> diff;
			std::set_difference(groups.begin(), groups.end(), m_GroupMovedThisTurn.begin(), m_GroupMovedThisTurn.end(), std::back_inserter(diff));

			for (unsigned int i = 0; i < diff.size(); i++)
			{
				Solidify(diff[i]);
			}

			m_GroupMovedThisTurn.clear();
		}
	}
}

void GameState::SetTurnTime(float turnTime)
{
	m_TurnTime = turnTime;
	m_TimeToNextTurn = m_TurnTime;
}

void GameState::ConnectClient(ITurnClient* client)
{
	m_TurnTimeSignal.connect(client, &ITurnClient::OnTurnTime);
}

void GameState::DisconnectClient(ITurnClient* client)
{
	m_TurnTimeSignal.disconnect(client);
}

void GameState::ReportFlowTileCreated(FlowTile* tile)
{
	m_FlowTiles.push_back(tile);
	ConnectClient(tile);
}

void GameState::ReportFlowTileDestroyed(FlowTile* tile)
{
	m_FlowTiles.erase(std::find(m_FlowTiles.begin(), m_FlowTiles.end(), tile));
	DisconnectClient(tile);
}

void GameState::MergeFlowGroups(int groupLhs, int groupRhs)
{
	for (auto tile : m_FlowTiles)
	{
		if (tile->GetFlowGroup() == groupRhs)
			tile->SetFlowGroup(groupLhs);
	}
}

void GameState::ReportTileSpread(FlowTile* tile)
{
	if (std::find(m_GroupMovedThisTurn.begin(), m_GroupMovedThisTurn.end(), tile->GetFlowGroup()) == m_GroupMovedThisTurn.end())
		m_GroupMovedThisTurn.push_back(tile->GetFlowGroup());
}

void GameState::GetTileGroups(std::vector<int>& groups)
{
	int group;
	for (auto tile : m_FlowTiles)
	{
		group = tile->GetFlowGroup();
		if (std::find(groups.begin(), groups.end(), group) == groups.end())
			groups.push_back(group);
	}
}

void GameState::Solidify(int group)
{
	for (auto tile : m_FlowTiles)
	{
		if (tile->GetFlowGroup() == group)
			tile->Solidify();
	}
}

void GameState::TriggerGameOver()
{

}