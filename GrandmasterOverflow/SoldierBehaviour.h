#ifndef SOLDIER_BEHAVIOUR_H
#define SOLDIER_BEHAVIOUR_H

#include "BoardUnit.h"
#include <SFML/System/Vector2.hpp>
#include "TeamState.h"

class RigidBody;
class NavAgent;

class SoldierBehaviour
	: public BoardUnit
{
public:
	SoldierBehaviour();

	void Awake() override;

	void Update() override;

	void OnCollisionEnter(CollisionData& collision) override;
private:
	watch_ptr<GameObject> m_Target;

	sf::Vector2f m_Objective;
	NavAgent* m_NavAgent;
};

#endif