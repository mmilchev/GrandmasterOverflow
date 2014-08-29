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

class TileState
	: public Component
{
public:
	TileState();

	void Awake() override;

	void SetTileType(TileType type);

	inline bool Occupied() const { return m_Occupant != nullptr; }
	inline void SetOccupant(GameObject* gObject) { m_Occupant = gObject; }
	inline TileType GetTileType() const { return m_TileType; };
	inline bool IsPassable() const { return m_TileType != Solid; }
private:
	TileType m_TileType;
	GameObject*	m_Occupant;

	static std::map<TileType, std::string> sTextureTile;
	static std::map<TileType, sf::Color> sColorTile;
};

#endif