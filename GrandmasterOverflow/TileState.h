#ifndef TILE_STATE_H
#define TILE_STATE_H

#include <Component.h>
#include <map>
#include <SFML/Graphics/Color.hpp>

enum TileType
{
	Empty = 0,
	Solid,
	Objective
};

class FlowTile;

class TileState
	: public Component
{
public:
	TileState();

	void Awake() override;

	void SetTileType(TileType type);

	void DestroyTile();

	inline bool Occupied() const { return m_Occupant != nullptr; }
	inline void SetOccupant(FlowTile* tile) { m_Occupant = tile; }
	inline FlowTile const* GetOccupant() const { return m_Occupant; }

	inline TileType GetTileType() const { return m_TileType; };
	inline bool IsPassable() const { return m_TileType != Solid; }
private:
	TileType m_TileType;
	FlowTile* m_Occupant;

	static std::map<TileType, std::string> sTextureTile;
	static std::map<TileType, sf::Color> sColorTile;
};

#endif