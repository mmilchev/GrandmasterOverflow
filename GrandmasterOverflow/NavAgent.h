#ifndef NAV_AGENT_H
#define NAV_AGENT_H
#include <SFML/System/Vector2.hpp>
#include <DynamicBehaviour.h>
#include "BoardMap.h"

class BoardUnit;

class NavAgent
	: public DynamicBehaviour
{
public:
	void OnDestruction() override;

public:
	NavAgent();

	void Awake() override;
	void Update() override;

	void SetSpeed(float speed) {}

	void SetDestination(sf::Vector2f const& dest);
	void SetRunning(bool running);
private:
	BoardUnit* m_Unit;
	BoardMap* m_Map;

	sf::Vector2f m_Destinaiton;
	bool m_Running;
};

#endif
