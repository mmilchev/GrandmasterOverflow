#ifndef PREFABS_H
#define PREFABS_H

#include <SFML/System/Vector2.hpp>
#include "GameState.h"

class GameObject;
class TargetPower;

namespace prefabs
{
	GameObject* CreateGameComponents(GameType type, int numOfTeam);

	GameObject* CreateGameCamera(sf::Vector2f const& pos);
	GameObject* CreateGUICamera();
	
	GameObject* CreateSidebar();
	GameObject* CreatePower(float size, sf::Vector2f const& startPos, sf::Vector2f const& targetPos);
	GameObject* CreateGhostPower(TargetPower* power);

	GameObject* CreateSoldier(sf::Vector2f const & pos, int team);
	GameObject* CreateBullet(GameObject* sender, sf::Vector2f const& pos, sf::Vector2f const& dir);

	GameObject* CreateEmptyBoard(int width, int height);
	void		LoadLevel(std::string const& name);
}

#endif