#include "FlowTile.h"
#include "BoardMap.h"
#include "Constants.h"

#include <GameObject.h>
#include <ConfigManager.h>
#include "Prefabs.h"
#include <Utils.h>
#include <GameTime.h>

FlowTile::FlowTile()
:m_ShouldSpread(true)
{
}

void FlowTile::OnDestruction()
{
	GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>()->DisconnectClient(this);
}

void FlowTile::Start()
{
	GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>()->ConnectClient(this);
	
	m_Board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	
	auto boardPos = m_Board->GetGridPos(m_GameObject->Transform()->Position());
	m_Board->SetOccupation(m_GameObject, boardPos);

	float scale = ConfigManager::GetFloat("[Flow Graphics]fStartSizeCoef");
	m_GameObject->Transform()->SetScale(sf::Vector2f(scale, scale));

	ResetTurns();
}

void FlowTile::Update()
{
	auto scale = m_GameObject->Transform()->Scale();
	m_GameObject->Transform()->SetScale(Lerp(scale, sf::Vector2f(1, 1), GameTime::DeltaTime() * ConfigManager::GetFloat("[Flow Graphics]fGrowSpeed")));
	if (m_ShouldSpread && m_TurnsLeft <= 0)
	{
		Spread();
		ResetTurns();
		m_ShouldSpread = true;
	}
}

void FlowTile::OnTurnTime()
{
	m_TurnsLeft--;
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
		if (state != nullptr
			&& !state->Occupied()
			&& state->IsPassable())
		{
			GameObject::Instantiate(prefabs::CreateFlow(m_Board->GetWorldPos(pos)));
		}
	}
}