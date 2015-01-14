#include "GameState.h"
#include "BoardMap.h"
#include "Constants.h"
#include "ITurnClient.h"
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
#include <SoundComponent.h>

GameState::GameState()
:m_TurnNum(0), m_GameOver(false)
{
}

void GameState::Start()
{
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	m_SoundComp = m_GameObject->GetComponent<SoundComponent>();

	m_SoundComp->GetSound().setVolume(ConfigManager::GetFloat("[Audio]fSoundEffectsVolume"));

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

		//Remove/Add any pending tiles
		m_FlowTiles.ProcessQueued();

		//Check for tile collisions and solidify the flows
		CheckAndSolidifyTiles();

		//Go through each tile and execute the logic of the turn
		ExecuteTurn();

		//If the tiles should spread on this turn
		int spreadPerTurns = ConfigManager::GetInt("[Flow Gameplay]iSpreadTurnTime");
		int logicTurns = m_TurnNum / spreadPerTurns;
		if (m_TurnNum % spreadPerTurns == 0)
		{
			std::vector<FlowTile::FlowTileType> types;
			GetTileTypes(types);

			//Check for remaining tile groups
			if (types.size() == 0)
			{
				//Game over if none left
				TriggerGameOver();
				return;
			}

			//Find the difference of what has moved this turn and any tiles currently remaining
			std::sort(types.begin(), types.end());
			std::sort(m_TypesMovedThisTurn.begin(), m_TypesMovedThisTurn.end());

			std::vector<FlowTile::FlowTileType> diff;
			std::set_difference(types.begin(), types.end(), m_TypesMovedThisTurn.begin(), m_TypesMovedThisTurn.end(), std::back_inserter(diff));

			//Solidify the difference (That means that a certain tile group cannot move any more)
			for (unsigned int i = 0; i < diff.size(); i++)
			{
				SolidifyTileType(diff[i]);
			}

			//Remove any tiles from the vector
			m_FlowTiles.ProcessQueued();

			m_TypesMovedThisTurn.clear();

			//Every note can be played multiple times
			int sample = std::min((logicTurns + 1) / ConfigManager::GetInt("[Audio]iPlaySameTurnSampleTimes"), ConfigManager::GetInt("[Audio]iMaxTurnSamples"));
			GameObject::Instantiate(prefabs::CreateNoteSample(sample));
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
	if (std::find(m_TypesMovedThisTurn.begin(), m_TypesMovedThisTurn.end(), tile->GetTileType()) == m_TypesMovedThisTurn.end())
		m_TypesMovedThisTurn.push_back(tile->GetTileType());
}

void GameState::GetTileTypes(std::vector<FlowTile::FlowTileType>& types)
{
	FlowTile::FlowTileType type;
	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		type = m_FlowTiles[i]->GetTileType();
		if (std::find(types.begin(), types.end(), type) == types.end())
			types.push_back(type);
	}
}

void GameState::SolidifyTileType(FlowTile::FlowTileType type)
{
	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		auto tile = m_FlowTiles[i];
		if (tile->GetTileType() == type)
			tile->Solidify();
	}
}

void GameState::TriggerGameOver()
{
	auto filled_prc = static_cast<int>(m_Board->GetPercentFilled());
	GameObject::Instantiate(prefabs::CreateLevelCompleteAnimation(filled_prc));
	m_SoundComp->PlayOneShot(filled_prc == 100 ? "success.ogg" : "failure.ogg");
	m_GameOver = true;
}

void GameState::CheckAndSolidifyTiles()
{
	std::vector<FlowTile::FlowTileType> typesForSolidifying;

	for (int i = 0; i < m_FlowTiles.Size(); ++i)
	{
		auto tile = m_FlowTiles[i];
		if (tile->CheckCollision() != nullptr || tile->IsOutOfMoves())
		{
			typesForSolidifying.push_back(tile->GetTileType());
		}
	}

	for (auto type : typesForSolidifying)
	{
		SolidifyTileType(type);
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
