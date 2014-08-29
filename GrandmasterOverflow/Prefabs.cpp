#include "Prefabs.h"
#include "Constants.h"
#include "TileState.h"
#include "BoardMap.h"
#include "GameState.h"
#include "GhostPower.h"
#include "PlaceTilePower.h"

#include <GameObject.h>
#include <SpriteRenderer.h>
#include <RigidBody.h>
#include <CircleCollider.h>
#include <TransformComponent.h>
#include <BoxCollider.h>
#include <Utils.h>
#include <ConfigManager.h>
#include <CameraComponent.h>
#include <InputInteractionComponent.h>
#include "SidebarBahaviour.h"
#include <iostream>
#include "pugixml.hpp"
#include <ResourceManager.h>
#include "CameraControl.h"

namespace prefabs
{
	GameObject* CreateGameComponents()
	{

		GameObject* gObject = new GameObject();
		{
			GameObject* gameStateObject = new GameObject();
			gameStateObject->SetTag(TAG_GAME_STATE);
			gameStateObject->SetName(NAME_GAME_STATE);
			gameStateObject->AddComponent(new GameState());
			gameStateObject->SetParent(gObject);
		}
		return gObject;
	}

	GameObject* CreateGameCamera(sf::Vector2f const& pos)
	{
		auto cameraObject = new GameObject();
		cameraObject->SetName(NAME_CAMERA);
		cameraObject->SetTag(TAG_GAME_CAMERA);

		auto comp = new CameraComponent();
		comp->SetLayerMask(Layer::Game | Layer::Default);
		cameraObject->AddComponent(comp);

		cameraObject->AddComponent(new CameraControl());

		cameraObject->Transform()->SetPosition(pos);

		return cameraObject;
	}

	GameObject* CreateGUICamera()
	{
		auto cameraObject = new GameObject();
		cameraObject->SetName(NAME_CAMERA);
		cameraObject->SetTag(TAG_GUI_CAMERA);

		auto comp = new CameraComponent();
		comp->SetLayerMask(Layer::GUI);
		cameraObject->AddComponent(comp);

		cameraObject->Transform()->SetPosition(sf::Vector2f(0, 0));

		return cameraObject;
	}

	GameObject* CreateSidebar()
	{
		auto gObject = new GameObject();
		gObject->SetTag(TAG_SIDEBAR);
		gObject->SetLayer(Layer::GUI);

		auto renderer = new SpriteRenderer("sidebarBackground.png");
		renderer->SetOrder(-1);
		gObject->AddComponent(renderer);

		auto sidebar = new SidebarBahaviour();
		sidebar->SetupParams();
		gObject->AddComponent(sidebar);

		gObject->AddComponent(new InputInteractionComponent());

		return gObject;
	}

	GameObject* CreatePower(float size, sf::Vector2f const& startPos, sf::Vector2f const& targetPos)
	{
		GameObject* gObject = new GameObject();
		gObject->SetTag(TAG_SLOT);
		gObject->SetLayer(Layer::GUI);

		auto renderer = new SpriteRenderer("solidGridTile.png");
		renderer->SetSpriteSize(sf::Vector2f(size, size));
		gObject->AddComponent(renderer);

		gObject->AddComponent(new InputInteractionComponent());

		auto power = new PlaceTilePower(targetPos);
		gObject->AddComponent(power);

		gObject->Transform()->SetPosition(startPos);

		return gObject;
	}

	GameObject* CreateGhostPower(TargetPower* power)
	{
		GameObject* gObject = new GameObject();
		gObject->SetTag(TAG_GHOST_POWER);
		gObject->SetLayer(Layer::Game);

		auto renderer = new SpriteRenderer("solidGridTile.png");
		renderer->SetSpriteSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		renderer->SetSpriteColor(sf::Color::Green);
		gObject->AddComponent(renderer);

		auto ghost = new GhostPower(power);
		gObject->AddComponent(ghost);

		return gObject;
	}

	GameObject* CreateEmptyBoard(int width, int height)
	{
		GameObject* gObject = new GameObject();
		gObject->SetName(NAME_GAME_BOARD);
		gObject->SetTag(TAG_GAME_BOARD);

		auto mapComp = new BoardMap(width, height);
		gObject->AddComponent(mapComp);

		//Add all the tiles
		auto startPos = -sf::Vector2f(width / 2.f, height / 2.f) * TILE_SIZE;
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				mapComp->CreateEmptyTile(sf::Vector2i(i, j));
			}
		}

		//Add walls on the sideds of the board to prevent units from leaving it
		{
			//Left wall
			GameObject* wall = new GameObject();
			wall->AddComponent(new BoxCollider(GRID_WALL_WIDTH, height * TILE_SIZE));
			auto body = new RigidBody();
			body->SetDefBodyType(b2_staticBody);
			wall->AddComponent(body);
			wall->Transform()->SetPosition(sf::Vector2f(startPos.x - GRID_WALL_WIDTH / 2, 0));
			wall->SetParent(gObject);
		}
		{
			//Right wall
			GameObject* wall = new GameObject();
			wall->AddComponent(new BoxCollider(GRID_WALL_WIDTH, height * TILE_SIZE));
			auto body = new RigidBody();
			body->SetDefBodyType(b2_staticBody);
			wall->AddComponent(body);
			wall->Transform()->SetPosition(sf::Vector2f(startPos.x + width * TILE_SIZE + GRID_WALL_WIDTH / 2, 0));
			wall->SetParent(gObject);
		}
		{
			//Top wall
			GameObject* wall = new GameObject();
			wall->AddComponent(new BoxCollider(width * TILE_SIZE, GRID_WALL_WIDTH));
			auto body = new RigidBody();
			body->SetDefBodyType(b2_staticBody);
			wall->AddComponent(body);
			wall->Transform()->SetPosition(sf::Vector2f(0, startPos.y - GRID_WALL_WIDTH / 2));
			wall->SetParent(gObject);
		}
		{
			//Bottom wall
			GameObject* wall = new GameObject();
			wall->AddComponent(new BoxCollider(width * TILE_SIZE, GRID_WALL_WIDTH));
			auto body = new RigidBody();
			body->SetDefBodyType(b2_staticBody);
			wall->AddComponent(body);
			wall->Transform()->SetPosition(sf::Vector2f(0, startPos.y + height * TILE_SIZE + GRID_WALL_WIDTH / 2));
			wall->SetParent(gObject);
		}

		return gObject;
	}

	void LoadLevel(std::string const& name)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(ResourceManager::GetResourceFilePath(ResourceManager::ksLevelFolder, name).c_str());

		if (!result)
		{
			std::cout << "XML [" << name << "] parsed with errors";
			return;
		}

		//GameScene init
		auto gameCamera = prefabs::CreateGameCamera(sf::Vector2f(0, 0));
		GameObject::Instantiate(gameCamera);
		auto guiCamera = prefabs::CreateGUICamera();
		GameObject::Instantiate(guiCamera);

		auto gameComponents = prefabs::CreateGameComponents();
		GameObject::Instantiate(gameComponents);

		GameObject::Instantiate(prefabs::CreateSidebar());

		pugi::xml_node levelNode = doc.child("level");

		int lWidth = static_cast<int>(levelNode.attribute("width").as_float() / TILE_SIZE);
		int lHeight = static_cast<int>(levelNode.attribute("height").as_float() / TILE_SIZE);

		sf::Vector2f worldTopLeft = sf::Vector2f(-lWidth * TILE_SIZE / 2, -lHeight * TILE_SIZE / 2);

		auto grid = prefabs::CreateEmptyBoard(lWidth, lHeight);
		auto board = grid->GetComponent<BoardMap>();

		//backgroundMidiTag = levelNode.attribute("BackgroundMIDI").as_string();
		auto tilesXML = levelNode.child("Tiles");
		for (auto& tileXML : tilesXML)
		{
			int x, y, id;
			x = tileXML.attribute("x").as_int();
			y = tileXML.attribute("y").as_int();
			id = tileXML.attribute("id").as_int();

			switch (id)
			{
			case 0:
				board->CreateSolidTile(sf::Vector2i(x, y));
				break;
			}
		}

		GameObject::Instantiate(grid);

		auto unitsXML = levelNode.child("Units");
		for (auto& unitXML : unitsXML)
		{
			float x, y;
			float angle;
			int team;
			auto name = std::string(unitXML.name());

			x = worldTopLeft.x + unitXML.attribute("x").as_float();
			y = worldTopLeft.y + unitXML.attribute("y").as_float();

			angle = unitXML.attribute("angle").as_float();

			team = unitXML.attribute("TeamN").as_int();

			if (name == "Soldier")
			{
				/*
				auto gObject = CreateSoldier(sf::Vector2f(x, y), team);
				gObject->Transform()->SetRotation(angle);
				GameObject::Instantiate(gObject);*/
			}
			else if (name == "Objective")
			{
				//gameComponents->GetChildWithName(NAME_TEAM + std::to_string(team))->GetComponent<TeamState>()->AddObjectiveSpot(sf::Vector2f(x, y));
			}
		}
	}
}