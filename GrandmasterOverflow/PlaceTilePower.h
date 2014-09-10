#ifndef PLACE_TILE_POWER_H
#define PLACE_TILE_POWER_H

#include "TargetPower.h"
#include "Constants.h"

class BoardMap;

class PlaceTilePower
	: public TargetPower
{
public:
	enum Type
	{
		Single,
		SingleDestroy,
		CornerSmall,
		CornerBig,
		IShapeSmall,
		IShapeBig,
		OShape,
		TShape
	};

	PlaceTilePower(Type type, int uses);

	void Awake() override;

	bool CanBePlaced(sf::Vector2i const& pos) override;
	void PopulateGhostPower() override;

protected:
	void UsePower(sf::Vector2i const& pos) override;

	Type m_Type;

	struct Tiles
	{
		enum TileType
		{
			Create,
			Destroy,
		};
		struct Tile
		{
			TileType m_Type;
			sf::Vector2f m_Pos;
		};

		sf::Vector2f m_GhostOrigin;
		std::vector<Tile> m_Tiles;

		void AddTile(TileType type, sf::Vector2f const& pos)
		{
			Tile tile;
			tile.m_Pos = pos;
			tile.m_Type = type;
			m_Tiles.push_back(tile);
		}
	} m_TileGrid;
};

#endif