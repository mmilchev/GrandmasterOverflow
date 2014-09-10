#ifndef TARGET_POWER_H
#define TARGET_POWER_H

#include "Power.h"

class GhostPower;
class BoardMap;
class GameState;

class TargetPower
	: public Power
{
public:
	explicit TargetPower(int uses);

	void Awake() override;

	void OnClicked() override;

	void CancelTarget();

	void OnPowerPlaced(sf::Vector2i const& pos);
	
	virtual bool CanBePlaced(sf::Vector2i const& pos) = 0;
	virtual void PopulateGhostPower() = 0;
protected:
	void TriggerPower();
	virtual void UsePower(sf::Vector2i const& pos) = 0;

	GhostPower* m_GhostPower;
	BoardMap* m_Board;
	GameState* m_GameState;
};

#endif