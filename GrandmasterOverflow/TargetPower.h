#ifndef TARGET_POWER_H
#define TARGET_POWER_H

#include "Power.h"

class GhostPower;
class BoardMap;

class TargetPower
	: public Power
{
public:
	explicit TargetPower(sf::Vector2f const& targetPos);

	void Awake() override;
	void OnClicked() override;

	void Update() override;

	void CancelTarget();

	void OnPowerPlaced(sf::Vector2i const& pos);
	
	virtual bool CanBePlaced(sf::Vector2i const& pos) = 0;
	virtual void PopulateGhostPower() = 0;
protected:
	virtual void UsePower(sf::Vector2i const& pos) = 0;

	GhostPower* m_GhostPower;
	BoardMap* m_Board;
};

#endif