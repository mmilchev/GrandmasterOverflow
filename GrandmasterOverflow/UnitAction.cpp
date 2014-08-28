#include "UnitAction.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <Utils.h>
#include "Prefabs.h"
#include <GameTime.h>
#include <ConfigManager.h>

UnitAction::UnitAction(GameObject* gObject)
:m_GameObject(gObject)
{
}

MoveAction::MoveAction(GameObject* gObject, sf::Vector2f const& destination)
: UnitAction(gObject), m_Destination(destination), m_Time(0)
{
	m_Origin = gObject->Transform()->Position();
}

void MoveAction::Update()
{
	m_Time += GameTime::DeltaTime();

	float logArg = ClampTop(1 + m_Time * 9 * 1.5f / ConfigManager::GetFloat("[Gameplay]fTurnTime"), 10);

	auto transform = m_GameObject->Transform();
	auto newPos = Lerp(m_Origin, m_Destination, log10(logArg));
	transform->SetPosition(newPos);
}

bool MoveAction::Done() const
{
	return Length(m_GameObject->Transform()->Position() - m_Destination) < 0.1f;
}

RotateAction::RotateAction(GameObject* gObject, float target)
:UnitAction(gObject), m_Target(target), m_Time(0)
{
	m_Origin = gObject->Transform()->Rotation();
}

void RotateAction::Update()
{
	m_Time += GameTime::DeltaTime();

	float logArg = ClampTop(1 + m_Time * 9 * 1.5f / ConfigManager::GetFloat("[Gameplay]fTurnTime"), 10);

	auto transform = m_GameObject->Transform();
	auto newRot = LerpAngle(m_Origin, m_Target, log10(logArg));
	transform->SetRotation(newRot);
}

bool RotateAction::Done() const
{
	return AreAnglesEqual(m_GameObject->Transform()->Rotation(), m_Target, 0.1f);
}

bool ShootAction::Done() const
{
	return true;
}

ShootAction::ShootAction(GameObject* gObject, sf::Vector2f const& dir)
:UnitAction(gObject)
{
	GameObject::Instantiate(prefabs::CreateBullet(m_GameObject, m_GameObject->Transform()->Position(), dir));
}