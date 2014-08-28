#ifndef BOARD_MAP_H
#define BOARD_MAP_H

#include <GameObject.h>
#include <TransformComponent.h>
#include <pathfinderastargeneric.h>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "TileState.h"
#include "Constants.h"

class GameObject;

namespace sf
{
	template<class VecType>
	bool operator<  (const sf::Vector2<VecType>& lhs, const sf::Vector2<VecType>& rhs)
	{
		return lhs.x<rhs.x || (!(rhs.x<lhs.x) && lhs.y<rhs.y);
	}
}

class BoardMap
	: public Component
{
private:
	class DestinationCost
	{
	public:
		double operator()(const sf::Vector2i& node, const sf::Vector2i& goal)
		{
			return abs(node.x - goal.x) + abs(node.y - goal.y);
		}
	};
	
	class GetNeighbors
	{
	public:
		GetNeighbors(BoardMap &themap) :m_Map(themap)
		{
		}

		inline void operator()(const sf::Vector2i &node, Pathfinder::AStarGeneric<sf::Vector2i, DestinationCost, GetNeighbors>::AddNeighborFunctor &an)
		{
			//4 dir movement for now
			sf::Vector2i pos;
			pos.x = node.x;
			for (pos.y = node.y - 1; pos.y <= node.y + 1; pos.y++)
			{
				if (pos.y != node.y	&& pos.y >= 0 && pos.y<m_Map.m_Height)
				{
					auto state = m_Map.GetTileState(pos);
					if (state->IsPassable())
					{
						an(pos, state->Occupied() ? 3 : 1);
					}
				}
			}
			pos.y = node.y;
			for (pos.x = node.x - 1; pos.x <= node.x + 1; pos.x++)
			{
				if (pos.x != node.x	&& pos.x >= 0 && pos.x < m_Map.m_Width)
				{
					auto state = m_Map.GetTileState(pos);
					if (state->IsPassable())
					{
						an(pos, state->Occupied() ? 3 : 1);
					}
				}
			}
		}
	private:

		BoardMap &m_Map;
	};
	
public:
	BoardMap(int width, int height);

	GameObject* CreateEmptyTile(const sf::Vector2i& pos);
	GameObject* CreateSolidTile(const sf::Vector2i& pos);

	std::vector<sf::Vector2i> FindPathTo(sf::Vector2f const& worldFrom, sf::Vector2f const& worldTo);
	std::vector<sf::Vector2f> CastPathToWorldSpace(std::vector<sf::Vector2i>const& path) const;

	void SetOccupation(GameObject* obj, sf::Vector2i const& pos);
	bool IsOccupied(sf::Vector2i const& pos);

	bool IsInBounds(sf::Vector2i const& pos);

	inline TileState* GetTileState(const sf::Vector2i& pos) { return GetTileObject(pos)->GetComponent<TileState>(); }
	inline GameObject* GetTileObject(const sf::Vector2i& pos) { return m_Tiles[pos.y * m_Width + pos.x]; }
	
	inline sf::Vector2f GetWorldPos(const sf::Vector2i& pos) const
	{
		return sf::Vector2f(TILE_SIZE * (pos.x + 0.5f - m_Width * 0.5f), TILE_SIZE * (pos.y + 0.5f - m_Height * 0.5f));
	}
	inline sf::Vector2i GetGridPos(const sf::Vector2f& pos) const
	{
		return sf::Vector2i(static_cast<int>(pos.x / TILE_SIZE + 0.5f * m_Width), static_cast<int>(pos.y / TILE_SIZE + 0.5f * m_Height));
	}
private:
	GameObject* CreateGenericTile(sf::Vector2i const& pos, TileType type);
	void RemoveTile(const sf::Vector2i& pos);

	//Map detailss
	std::vector<GameObject*> m_Tiles;
	int	m_Width, m_Height;
	sf::Vector2f m_WorldOriginPosition;

	//Pathfinder
	Pathfinder::AStarGeneric<sf::Vector2i, DestinationCost, GetNeighbors> m_Pathfinder;
	DestinationCost	m_DestinationCost;
	GetNeighbors m_GetNeighbors;
};

#endif