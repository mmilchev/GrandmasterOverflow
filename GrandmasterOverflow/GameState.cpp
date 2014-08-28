#include "GameState.h"
#include "TeamState.h"
#include "BoardMap.h"
#include "BoardUnit.h"
#include "Constants.h"
#include "SidebarBahaviour.h"

#include <SFML/System/Vector2.hpp>
#include <GameObject.h>
#include <CameraComponent.h>
#include <GameTime.h>
#include <ConfigManager.h>
#include <SFML/Window.hpp>
#include <SpriteRenderer.h>
#include <Application.h>

void GameState::Start()
{	
	SetTurnTime(ConfigManager::GetFloat("[Gameplay]fTurnTime"));
}

void GameState::Update()
{
	m_TimeToNextTurn -= GameTime::DeltaTime();
	if (m_TimeToNextTurn <= 0)
	{
		m_TimeToNextTurn += m_TurnTime;
		m_TurnTimeSignal.emit();
	}
}

void GameState::SetTurnTime(float turnTime)
{
	m_TurnTime = turnTime;
}

void GameState::ConnectUnit(BoardUnit* unit)
{
	m_TurnTimeSignal.connect(unit, &BoardUnit::OnTurnTime);
}

void GameState::ConnectSidebar(SidebarBahaviour* sidebar)
{
	m_TurnTimeSignal.connect(sidebar, &SidebarBahaviour::OnTurnTime);
}

void GameState::DisconnectUnit(BoardUnit* unit)
{
	m_TurnTimeSignal.disconnect(unit);
}

GameState::GameState(GameType type)
:m_GameType(type), m_TimeToNextTurn(1), m_TurnTime(1)
{
}