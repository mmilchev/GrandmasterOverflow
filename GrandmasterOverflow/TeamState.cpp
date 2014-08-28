#include "TeamState.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Constants.h"
#include "IObjectiveChooser.h"

#include <GameObject.h>
#include <SceneModule.h>

TeamState::TeamState(int teamNum)
:m_TeamNum(teamNum), m_ObjectiveChooser(nullptr)
{
}

void TeamState::Awake()
{
	SetGameMode(GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>()->GetGameType());
}

void TeamState::Update()
{
	//Erase deleted objects
	while (true)
	{
		auto itter = std::find_if(m_SeenSpots.begin(), m_SeenSpots.end(),
			[&](UnitSpot const& spot) { return  spot.m_Unit.get() == nullptr; });
		if (itter == m_SeenSpots.end())
		{
			break;
		}
		else
		{
			m_SeenSpots.erase(itter);
		}
	}
}

sf::Vector2f TeamState::GetClosestObjective(sf::Vector2f const& position) const
{
	return m_ObjectiveChooser->GetClosestObjective(*this, position);
}

void TeamState::OnUnitSpotted(GameObject* gObject)
{
	auto found = std::find_if(m_SeenSpots.begin(), m_SeenSpots.end(),
		[&gObject](UnitSpot const& spot) { return  spot.m_Unit.get() == gObject; });
	if (found == m_SeenSpots.end())
	{
		m_SeenSpots.push_back(UnitSpot(gObject));
	}
	else
	{
		found->m_Spot = gObject->Transform()->Position();
	}
}

void TeamState::AddObjectiveSpot(sf::Vector2f const& spot)
{
	m_ObjectiveSpots.push_back(spot);
}

void TeamState::SetGameMode(GameType type)
{
	switch (type)
	{
	case Deathmatch:
		m_ObjectiveChooser = std::unique_ptr<IObjectiveChooser>(new DeathmatchChooser);
		break;
	case ReachGoal: 
		m_ObjectiveChooser = std::unique_ptr<IObjectiveChooser>(new ReachGoalChooser);
		break;
	default: break;
	}
}

TeamState::UnitSpot::UnitSpot(GameObject* obj)
: m_Unit(obj)
{
	m_Spot = obj->Transform()->Position();
}