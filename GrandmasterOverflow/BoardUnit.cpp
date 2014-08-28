#include "BoardUnit.h"
#include "TeamState.h"
#include "Constants.h"
#include "BoardMap.h"

#include <GameObject.h>
#include <SpriteRenderer.h>
#include <Utils.h>
#include <PhysicsModule.h>
#include <TransformComponent.h>
#include <ConfigManager.h>
#include <GameTime.h>
#include <RigidBody.h>


BoardUnit::BoardUnit()
:m_TeamNum(-1), m_TurnsPerAction(1), m_CurrentTurn(0)
{
}

bool BoardUnit::GetClosestUnitSeen(GameObject*& gameObject)
{
	gameObject = nullptr;

	auto pos = m_GameObject->Transform()->Position();
	auto gObjects = GameObject::FindObjectsByTag(TAG_UNIT);

	//Add the spots to cast rays from
	std::vector<std::pair<GameObject*, ClosestUnit>> raycastData;
	for (auto raycastSpot : m_RaycastSpots)
	{
		raycastData.push_back(std::pair<GameObject*, ClosestUnit>(raycastSpot, ClosestUnit()));
	}

	float savedRotation = m_GameObject->Transform()->Rotation();
	int team = GetTeam();
	for (const auto& gObject : gObjects)
	{
		//Only consider seeing an object if it is not in the same team and is not *this
		if (gObject != m_GameObject
			&& gObject->GetComponent<BoardUnit>()->GetTeam() != GetTeam())
		{
			if (CanSeeUnit(gObject, raycastData)
				&& gObject->GetComponent<BoardUnit>()->GetTeam() != m_TeamNum)
			{
				gameObject = gObject;
				break;
			}
		}
	}
	m_GameObject->Transform()->SetRotation(savedRotation);

	return gameObject != nullptr;
}

bool BoardUnit::CanSeeUnit(GameObject* gameObject)
{
	float savedRotation = m_GameObject->Transform()->Rotation();
	
	//Add the spots to cast rays from
	std::vector<std::pair<GameObject*, ClosestUnit>> raycastData;
	for (auto raycastSpot : m_RaycastSpots)
	{
		raycastData.push_back(std::pair<GameObject*, ClosestUnit>(raycastSpot, ClosestUnit()));
	}
	bool result = CanSeeUnit(gameObject, raycastData);
	
	m_GameObject->Transform()->SetRotation(savedRotation);
	
	return result;
}

bool BoardUnit::CanSeeUnit(GameObject* gameObject, std::vector<std::pair<GameObject*, ClosestUnit>>& raycastData)
{
	auto pos = m_GameObject->Transform()->Position();

	//Set the rotation in the correct direction so that the positions of the spots are accurate
	auto gPos = gameObject->Transform()->Position();
	auto dirVec = gPos - pos;
	auto dir = Normalize(dirVec);
	auto length = Length(dirVec);
	m_GameObject->Transform()->SetRotation(GetRotationAngle(dir));

	bool failed = false;
	for (auto& rayData : raycastData)
	{
		auto& closestUnit = rayData.second;
		auto raycastSpot = rayData.first;
		closestUnit.Reset(-1);

		sf::Vector2f raycastPos = raycastSpot->Transform()->Position();

		PhysicsModule::RayCast(&closestUnit, raycastPos, raycastPos + dir * length);
		if (!closestUnit.HasData() || closestUnit.GetGameObject() != gameObject)
		{
			failed = true;
			break;
		}
	}

	return	!failed;
}

bool BoardUnit::IsActionRunning() const
{
	return m_CurrentAction.get() != nullptr;
}

void BoardUnit::ExecuteAction(UnitAction* action)
{
	m_CurrentAction.reset(action);
	m_CurrentTurn = 0;
}

bool BoardUnit::IsReadyForAction() const
{
	return m_CurrentTurn >= m_TurnsPerAction;
}

void BoardUnit::OnTurnTime()
{
	m_CurrentTurn++;
}

void BoardUnit::Awake()
{
	SetTeam(m_TeamNum);

	m_RaycastSpots = m_GameObject->GetChildrenWithTag(TAG_RAYCAST_SPOT);
	m_Map = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	m_GameState = GameObject::FindByTag(TAG_GAME_STATE)->GetComponent<GameState>();
	m_GameState->ConnectUnit(this);
}

void BoardUnit::Update()
{
	if (IsActionRunning())
	{
		m_CurrentAction->Update();
		if (m_CurrentAction->Done())
			m_CurrentAction = nullptr;
	}
}

void BoardUnit::OnDestruction()
{
	m_GameState->DisconnectUnit(this);
}

void BoardUnit::SetTeam(int teamNum)
{
	m_TeamNum = teamNum;
	if (m_GameObject != nullptr)
	{
		m_GameObject->GetChildWithTag(TAG_UNIT_GLOW)->GetComponent<SpriteRenderer>()->SetSpriteColor(m_TeamNum == 0 ? sf::Color::Green : sf::Color::Red);
		m_TeamState = GameObject::FindByName(NAME_TEAM + std::to_string(m_TeamNum))->GetComponent<TeamState>();
	}
}



float32 BoardUnit::ClosestUnit::ReportFixture(b2Fixture* fixture, b2Vec2 const& point, b2Vec2 const& normal, float32 fraction)
{
	auto gObject = GetGameObject(fixture);
	auto teamState = gObject->GetComponent<BoardUnit>();
	if (fixture->IsSensor() ||
		(teamState != nullptr && teamState->GetTeam() == m_IgnoreTeam))
	{
		return -1; //filter
	}

	m_ClosestFixture = fixture;
	m_ClosestPoint = FromBox2DWorld(BoxToSf(point));
	m_ClosestNormal = BoxToSf(normal);

	return fraction;
}

void BoardUnit::ClosestUnit::Reset(int ignoreTeam)
{
	m_IgnoreTeam = ignoreTeam;
	m_ClosestFixture = nullptr;
	m_ClosestPoint = sf::Vector2f(0, 0);
	m_ClosestNormal = sf::Vector2f(0, 0);
}

bool BoardUnit::ClosestUnit::HasData()
{
	return m_ClosestFixture != nullptr && GetGameObject(m_ClosestFixture)->GetTag() == TAG_UNIT;
}

GameObject* BoardUnit::ClosestUnit::GetGameObject()
{
	return GetGameObject(m_ClosestFixture);
}

GameObject* BoardUnit::ClosestUnit::GetGameObject(b2Fixture* fixture)
{
	return static_cast<GameObject*>(fixture->GetUserData());
}