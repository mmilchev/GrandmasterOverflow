#ifndef PREFABS_H
#define PREFABS_H

#include <SFML/System/Vector2.hpp>
#include "GameState.h"
#include "TileState.h"
#include "FlowTile.h"

class GameObject;
class TargetPower;

namespace prefabs
{
	GameObject* CreateGameComponents();

	GameObject* CreateGameCamera(sf::Vector2f const& pos);
	GameObject* CreateGUICamera();
	
	GameObject* CreateSidebar();
	GameObject* CreatePower(float size, sf::Vector2f const& startPos, sf::Vector2f const& targetPos);
	GameObject* CreateGhostPower(TargetPower* power);
	
	GameObject* CreateTile(sf::Vector2f const& pos, TileType type);
	
	GameObject* CreateFlow(sf::Vector2f const& pos, FlowTile::FlowTileType type, int group);

	GameObject* CreateTileDestroyer(sf::Vector2f const & pos);

	GameObject* CreateEmptyBoard(int width, int height);
	void		LoadLevel(std::string const& name);
}

#endif