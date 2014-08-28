#include "SoldierBehaviour.h"
#include "GameObject.h"
#include "Constants.h"
#include "BoardMap.h"
#include "NavAgent.h"
#include "Prefabs.h"
#include "BulletBehaviour.h"

#include <Utils.h>
#include <SFML/Graphics.hpp>
#include <ConfigManager.h>
#include <RigidBody.h>
#include <GameTime.h>
#include <TransformComponent.h>
#include <Application.h>
#include <CollisionData.h>

SoldierBehaviour::SoldierBehaviour()
:m_Target(nullptr)
{
}

void SoldierBehaviour::Awake()
{
	BoardUnit::Awake();
	m_NavAgent = m_GameObject->GetComponent<NavAgent>();
}

void SoldierBehaviour::Update()
{
	BoardUnit::Update();

	auto pos = m_GameObject->Transform()->Position();

	//Check if old target is still available
	if (m_Target == nullptr ||
		(m_Target != nullptr && !CanSeeUnit(m_Target.get())))
	{
		GameObject* gObject;
		GetClosestUnitSeen(gObject);
		m_Target = gObject;
	}

	if (m_Target != nullptr)
	{
		m_TeamState->OnUnitSpotted(m_Target.get());

		m_NavAgent->SetRunning(false);
		if (IsReadyForAction())
		{
			auto targetPos = m_Target->Transform()->Position();
			auto dir = Normalize(targetPos - pos);

			auto targetRot = GetRotationAngle(dir);
			auto curRot = m_GameObject->Transform()->Rotation();

			if (!AreAnglesEqual(curRot, targetRot, 5))
				ExecuteAction(new RotateAction(m_GameObject, targetRot));
			else
				ExecuteAction(new ShootAction(m_GameObject, dir));
		}
	}
	else
	{
		auto target = m_TeamState->GetClosestObjective(pos);
		if (target != m_Objective)
		{
			m_NavAgent->SetDestination(target);
			m_Objective = target;
		}
		else
		{
			m_NavAgent->SetRunning(true);
		}
	}
}

void SoldierBehaviour::OnCollisionEnter(CollisionData& collision)
{
	if (collision.m_GameObject->GetTag() == TAG_BULLET &&
		collision.m_GameObject->GetComponent<BulletBehaviour>()->GetSender() != m_GameObject)
		GameObject::Destroy(m_GameObject);
}