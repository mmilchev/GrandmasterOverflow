#ifndef BOARD_MAP_H
#define BOARD_MAP_H

#include <GameObject.h>
#include <TransformComponent.h>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "TileState.h"
#include "Constants.h"

class GameObject;

class BoardMap
	: public Component
{
public:
	BoardMap(int width, int height);

	GameObject* CreateEmptyTile(const sf::Vector2i& pos);
	GameObject* CreateSolidTile(const sf::Vector2i& pos);
	GameObject* CreateObjectiveTile(const sf::Vector2i pos);

	void SetOccupation(FlowTile* tile, sf::Vector2i const& pos);
	bool IsOccupied(sf::Vector2i const& pos);

	bool IsInBounds(sf::Vector2i const& pos);

	inline TileState* GetTileState(sf::Vector2i const& pos)
	{
		if (!IsInBounds(pos))
			return nullptr;
		return m_Tiles[pos.y * m_Width + pos.x];
	}
	inline GameObject* GetTileObject(sf::Vector2i const& pos)
	{
		TileState* state = GetTileState(pos);
		
		if (state == nullptr)
			return nullptr;

		return state->GetGameObject();
	}
	
	inline sf::Vector2f GetWorldPos(sf::Vector2i const& pos) const
	{
		return sf::Vector2f(TILE_SIZE * (pos.x + 0.5f - m_Width * 0.5f), TILE_SIZE * (pos.y + 0.5f - m_Height * 0.5f));
	}
	inline sf::Vector2i GetGridPos(sf::Vector2f const& pos) const
	{
		return sf::Vector2i(static_cast<int>(pos.x / TILE_SIZE + 0.5f * m_Width), static_cast<int>(pos.y / TILE_SIZE + 0.5f * m_Height));
	}
private:
	GameObject* CreateGenericTile(sf::Vector2i const& pos, TileType type);
	void RemoveTile(const sf::Vector2i& pos);

	//Map detailss
	std::vector<TileState*> m_Tiles;
	int	m_Width, m_Height;
	sf::Vector2f m_WorldOriginPosition;
};

#endif