#include "GameState.h"
#include "BoardMap.h"
#include "Constants.h"
#include "SidebarBahaviour.h"
#include "ITurnClient.h"

#include <SFML/System/Vector2.hpp>
#include <GameObject.h>
#include <CameraComponent.h>
#include <GameTime.h>
#include <ConfigManager.h>
#include <SFML/Window.hpp>
#include <SpriteRenderer.h>
#include <Application.h>

GameState::GameState()
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
		m_TimeToNextTurn += m_TurnTime;
		m_TurnTimeSignal.emit();
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