#ifndef PREFABS_H
#define PREFABS_H

#include <SFML/System/Vector2.hpp>
#include <functional>
#include "GameState.h"
#include "TileState.h"
#include "FlowTile.h"
#include "PlaceTilePower.h"

class GameObject;
class TargetPower;

namespace prefabs
{
	GameObject* CreateGameComponents();

	GameObject* CreateGameCamera(sf::Vector2f const& pos);
	GameObject* CreateGUICamera();
	
	GameObject* CreatePower(float size, sf::Vector2f const& pos, std::string const& textureName, int uses);
	GameObject* CreatePlaceTilePower(float size, sf::Vector2f const& pos, PlaceTilePower::Type type, int uses);
	GameObject* CreateGhostPower(TargetPower* power);

	GameObject* CreateTimeMenu(float height);
	
	GameObject* CreateTile(sf::Vector2f const& pos, TileType type);
	
	GameObject* CreateFlow(sf::Vector2f const& pos, FlowTile::FlowTileType type, int group, int turns);

	GameObject* CreateIconAnimation(std::string const& textureName);
	GameObject* CreateIconAnimation(std::string const& textureName, float spriteSize);

	GameObject* CreateLevelCompleteAnimation(int percentComplete);
	GameObject* CreateMessageAnimation(std::string const& message, std::function<void()> msgEndPred);

	GameObject* CreateTileDestroyer(sf::Vector2f const & pos);

	GameObject* CreateEmptyBoard(int width, int height);
	void		LoadLevel(std::string const& name);
}

#endif