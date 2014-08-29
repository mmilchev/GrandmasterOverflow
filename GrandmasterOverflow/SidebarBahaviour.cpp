#include "SidebarBahaviour.h"
#include "Constants.h"
#include "GameState.h"
#include "Prefabs.h"
#include "Power.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <Application.h>
#include <Utils.h>
#include <ConfigManager.h>
#include <SpriteRenderer.h>

SidebarBahaviour::SidebarBahaviour()
:m_TurnsBeforeBatch(1), m_CurrentTurn(0)
{
}

void SidebarBahaviour::OnMouseEnter()
{
	auto sSize = Application::GetWindow().getSize();
	m_TargetPos = sf::Vector2f((sSize.x - m_Width) / 2, 0);
}

void SidebarBahaviour::OnMouseLeave()
{
	auto sSize = Application::GetWindow().getSize();
	m_TargetPos = sf::Vector2f((sSize.x + m_Width) / 2 - m_LeftBorder, 0);
}

void SidebarBahaviour::SetupParams()
{
	auto sSize = Application::GetWindow().getSize();
	m_Border = ConfigManager::GetFloat("[GUI]fBorderHeightCoef") * sSize.y;
	m_LeftBorder = ConfigManager::GetFloat("[GUI]fSidebarLeftBorderCoef") * sSize.y;
	m_ActivePowerNum = ConfigManager::GetInt("[Gameplay]iActiveBlockNum");
	m_PowerSquareSize = (sSize.y - (m_ActivePowerNum + 1) * m_Border) / m_ActivePowerNum;
	auto maxSize = ConfigManager::GetFloat("[GUI]fMaxBlockSizeCoef") * sSize.x;
	m_PowerSquareSize = ClampTop(m_PowerSquareSize, maxSize);
	m_Width = m_PowerSquareSize + 2 * m_Border + m_LeftBorder;

	m_TurnsBeforeBatch = ConfigManager::GetInt("[Gameplay]iTurnsBeforePowerBatch");
}

void SidebarBahaviour::Awake()
{
	SetupParams();

	auto sSize = Application::GetWindow().getSize();
	//Align right
	m_GameObject->GetComponent<SpriteRenderer>()->SetSpriteSize(sf::Vector2f(m_Width, static_cast<float>(sSize.y)));
	
	m_TargetPos = sf::Vector2f((sSize.x + m_Width) / 2 - m_LeftBorder, 0);
	m_GameObject->Transform()->SetPosition(m_TargetPos);

	GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>()->ConnectClient(this);

	for (int i = 0; i < m_ActivePowerNum; ++i)
	{
		m_AvailablePowers[i] = nullptr;
	}
	FillEmptyPowers();
}

void SidebarBahaviour::Update()
{
	if (m_CurrentTurn >= m_TurnsBeforeBatch)
	{
		FillEmptyPowers();
		m_CurrentTurn = 0;
	}

	auto pos = m_GameObject->Transform()->Position();
	auto delta = m_TargetPos - pos;
	if (Length(delta) > 1)
		m_GameObject->Transform()->SetPosition(pos + delta * GameTime::DeltaTime() * ConfigManager::GetFloat("[GUI]fSidebarOpenSpeed"));
}

void SidebarBahaviour::OnTurnTime()
{
	m_CurrentTurn++;
}

void SidebarBahaviour::FillEmptyPowers()
{
	auto sSize = Application::GetWindow().getSize();

	for (auto& pair : m_AvailablePowers)
	{
		if (pair.second == nullptr)
		{
			auto pos = m_GameObject->Transform()->Position();
			pos.y = 0;	
			auto gObject = prefabs::CreatePower(m_PowerSquareSize, pos, sf::Vector2f(m_LeftBorder / 2,	
				-(sSize.y / 2.f) + m_Border + m_PowerSquareSize / 2 + (m_Border + m_PowerSquareSize) * pair.first));
			gObject->SetParent(m_GameObject);
			GameObject::Instantiate(gObject);

			pair.second = gObject;
		}
	}
}