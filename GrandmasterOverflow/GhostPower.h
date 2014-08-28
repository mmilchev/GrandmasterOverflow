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
private:

	TargetPower* m_Power;
	BoardMap* m_Board;
};

#endif