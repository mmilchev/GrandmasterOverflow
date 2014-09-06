#include "BoardMap.h"
#include "TileState.h"
#include "Prefabs.h"

#include <GameObject.h>


BoardMap::BoardMap(int width, int height)
: m_Width(width), m_Height(height), 
m_WorldOriginPosition(-sf::Vector2f(width / 2.f, height / 2.f) * TILE_SIZE)
{
	//Init empty board
	for (int i = 0; i < width * height; ++i)
	{
		m_Tiles.push_back(nullptr);
	}
}

GameObject* BoardMap::CreateEmptyTile(sf::Vector2i const& pos)
{
	auto tile = CreateGenericTile(pos, Empty);
	GameObject::Instantiate(tile);
	return tile;
}

GameObject* BoardMap::CreateSolidTile(sf::Vector2i const& pos)
{
	auto tile = CreateGenericTile(pos, Solid);

	GameObject::Instantiate(tile);

	return tile;
}

void BoardMap::SetOccupation(FlowTile* obj, sf::Vector2i const& pos)
{
	GetTileState(pos)->SetOccupant(obj);
}

bool BoardMap::IsOccupied(sf::Vector2i const& pos)
{
	return GetTileState(pos)->Occupied();
}

bool BoardMap::IsInBounds(sf::Vector2i const& pos)
{
	return pos.x >= 0 &&
		pos.x < m_Width &&
		pos.y >= 0 &&
		pos.y < m_Height;
}

bool BoardMap::Contains(sf::Vector2f const& pos)
{
	return IsInBounds(GetGridPos(pos));
}

void BoardMap::OnClicked()
{
}

GameObject* BoardMap::CreateGenericTile(sf::Vector2i const& pos, TileType type)
{
	RemoveTile(pos);

	auto tile = prefabs::CreateTile(m_WorldOriginPosition + sf::Vector2f(TILE_SIZE / 2 + pos.x * TILE_SIZE, TILE_SIZE / 2 + pos.y * TILE_SIZE), type);
	tile->SetParent(m_GameObject);
	m_Tiles[pos.y * m_Width + pos.x] = tile->GetComponent<TileState>();

	return tile;
}

void BoardMap::RemoveTile(sf::Vector2i const& pos)
{
	auto tile = GetTileObject(pos);
	if (tile != nullptr)
		GameObject::Destroy(tile);
}