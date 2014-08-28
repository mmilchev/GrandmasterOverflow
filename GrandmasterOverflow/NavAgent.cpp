#include "NavAgent.h"
#include "BoardMap.h"
#include "BoardUnit.h"

#include <GameObject.h>
#include <RigidBody.h>
#include <Utils.h>
#include "UnitAction.h"

void NavAgent::OnDestruction()
{
	m_Map->SetOccupation(nullptr, m_Map->GetGridPos(m_GameObject->Transform()->Position()));
}

NavAgent::NavAgent()
{
}

void NavAgent::Awake()
{
	m_Unit = m_GameObject->GetComponent<BoardUnit>();
	m_Map = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
}

void NavAgent::Update()
{
	if (!m_Running)
		return;

	auto pos = m_GameObject->Transform()->Position();
	if (Length(m_Destinaiton - pos) > 0.01f)
	{
		if (m_Unit->IsReadyForAction())
		{
			auto path = m_Map->CastPathToWorldSpace(m_Map->FindPathTo(pos, m_Destinaiton));
			sf::Vector2f nextStep = pos;
			if (path.size() > 1)
				nextStep = path[path.size() - 2];
			else if (path.size() > 0)
				nextStep = path[0];

			auto dir = Normalize(nextStep - pos);
			auto rotation = GetRotationAngle(dir);
			auto curRotation = m_GameObject->Transform()->Rotation();

			if (!AreAnglesEqual(curRotation, rotation, 2))
			{
				m_Unit->ExecuteAction(new RotateAction(m_GameObject, rotation));
			}
			else if (!m_Map->IsOccupied(m_Map->GetGridPos(nextStep)))
			{
				m_Unit->ExecuteAction(new MoveAction(m_GameObject, nextStep));
				m_Map->SetOccupation(nullptr, m_Map->GetGridPos(pos));
				m_Map->SetOccupation(m_GameObject, m_Map->GetGridPos(nextStep));
			}
		}
	}
	else
	{
		SetRunning(false);
	}
}

void NavAgent::SetDestination(sf::Vector2f const& dest)
{
	m_Destinaiton = dest;
	m_Running = true;
}

void NavAgent::SetRunning(bool running)
{
	m_Running = running;
}