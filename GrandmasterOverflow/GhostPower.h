#ifndef GHOST_POWER_H
#define GHOST_POWER_H

#include <SFML/System/Vector2.hpp>
#include <DynamicBehaviour.h>

class TargetPower;
class BoardMap;

class GhostPower
	: public DynamicBehaviour
{
public:
	explicit GhostPower(TargetPower* power);

	void Awake() override;
	void Update() override;

	inline void SetOrigin(sf::Vector2f const& origin) { m_Origin = origin; }
private:

	sf::Vector2f m_Origin;

	TargetPower* m_Power;
	BoardMap* m_Board;
};

#endif