#ifndef PLACE_TILE_POWER_H
#define PLACE_TILE_POWER_H

#include "TargetPower.h"

class BoardMap;

class PlaceTilePower
	: public TargetPower
{
public:
	explicit PlaceTilePower(sf::Vector2f const& targetPos);

	bool CanBePlaced(sf::Vector2i const& pos) override;
	void PopulateGhostPower() override;

protected:
	void UsePower(sf::Vector2i const& pos) override;
};

#endif