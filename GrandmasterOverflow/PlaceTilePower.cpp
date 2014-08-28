#include "PlaceTilePower.h"
#include "BoardMap.h"

PlaceTilePower::PlaceTilePower(sf::Vector2f const& targetPos)
: TargetPower(targetPos)
{
}

bool PlaceTilePower::CanBePlaced(sf::Vector2i const& pos)
{
	return !m_Board->IsOccupied(pos) && m_Board->GetTileState(pos)->GetTileType() == TileType::Empty;
}

void PlaceTilePower::PopulateGhostPower()
{
}

void PlaceTilePower::UsePower(sf::Vector2i const& pos)
{
	m_Board->CreateSolidTile(pos);
}