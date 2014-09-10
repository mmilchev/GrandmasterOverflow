#include "Prefabs.h"
#include "Constants.h"
#include "TileState.h"
#include "BoardMap.h"
#include "GameState.h"
#include "GhostPower.h"
#include "PlaceTilePower.h"
#include "pugixml.hpp"
#include "CameraControl.h"
#include "FlowTile.h"
#include "TileDestroyer.h"

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
#include <iostream>
#include <ResourceManager.h>
#include <TextRenderer.h>
#include <Application.h>
#include <BoxInteractionComponent.h>

namespace prefabs
{
	std::map<PlaceTilePower::Type, std::string> kPowerTextures =
	{
		{ PlaceTilePower::Type::Single, "powerSingle.png" },
		{ PlaceTilePower::Type::SingleDestroy, "powerSingleDestroy.png" },
		{ PlaceTilePower::Type::CornerSmall, "powerCornerSmall.png" },
		{ PlaceTilePower::Type::CornerBig, "powerCornerBig.png" },
		{ PlaceTilePower::Type::IShapeSmall, "powerIshapeSmall.png" },
		{ PlaceTilePower::Type::IShapeBig, "powerIShapeBig.png" },
		{ PlaceTilePower::Type::TShape, "powerTShape.png" },
		{ PlaceTilePower::Type::OShape, "powerOShape.png" },
	};

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

	GameObject* CreatePower(float size, sf::Vector2f const& pos, PlaceTilePower::Type type, int uses)
{
		GameObject* gObject = new GameObject();
		gObject->SetTag(TAG_SLOT);
		gObject->SetLayer(Layer::GUI);

		auto innerIcon = new GameObject();
		innerIcon->SetName(NAME_ICON_POWER);
		innerIcon->SetTag(TAG_SLOT);
		innerIcon->SetLayer(Layer::GUI);

		auto renderer = new SpriteRenderer(kPowerTextures[type]);
		renderer->SetSpriteSize(sf::Vector2f(size, size));
		innerIcon->AddComponent(renderer);

		innerIcon->SetParent(gObject);
		innerIcon->Transform()->SetLocalPosition(sf::Vector2f(-size/2, 0));

		auto innerText = new GameObject();
		innerText->SetName(NAME_TEXT_POWER);
		innerText->SetTag(TAG_SLOT);
		innerText->SetLayer(Layer::GUI);

		auto textRenderer = new TextRenderer();
		textRenderer->Text().setFont(ResourceManager::GetFont("font.ttf"));
		textRenderer->Text().setCharacterSize(static_cast<int>(3*size / 5));
		textRenderer->Text().setColor(sf::Color::White);
		textRenderer->Text().setString("x" + std::to_string(uses));
		textRenderer->SetAlignment(TextRenderer::Center);
		innerText->AddComponent(textRenderer);

		innerText->SetParent(gObject);
		innerText->Transform()->SetLocalPosition(sf::Vector2f(size / 2, 0));

		int intSize = static_cast<int>(size);
		gObject->AddComponent(new BoxInteractionComponent(2 * intSize, intSize));

		gObject->AddComponent(new InputInteractionComponent());

		auto power = new PlaceTilePower(type, uses);
		gObject->AddComponent(power);

		gObject->Transform()->SetPosition(pos);

		return gObject;
	}

	GameObject* CreateGhostPower(TargetPower* power)
	{
		GameObject* gObject = new GameObject();
		gObject->SetTag(TAG_GHOST_POWER);
		gObject->SetLayer(Layer::Game);

		auto ghost = new GhostPower(power);
		gObject->AddComponent(ghost);

		return gObject;
	}

	GameObject* CreateTile(sf::Vector2f const& pos, TileType type)
	{
		GameObject* tile = new GameObject();
		tile->SetTag(TAG_TILE);
		tile->SetLayer(Layer::Game);

		auto renderer = new SpriteRenderer("emptyGridTile.png");
		renderer->SetOrder(-100);
		renderer->SetSpriteSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		tile->AddComponent(renderer);
		auto state = new TileState();
		state->SetTileType(type);
		tile->AddComponent(state);
		tile->Transform()->SetPosition(pos);

		return tile;
	}

	GameObject* CreateFlow(sf::Vector2f const& pos, FlowTile::FlowTileType type, int group, int turns)
{
		GameObject* gObject = new GameObject();
		gObject->SetTag(TAG_FLOW);
		gObject->SetLayer(Layer::Game);

		auto renderer = new SpriteRenderer("solidGridTile.png");
		renderer->SetSpriteSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		renderer->SetSpriteColor(sf::Color::Green);
		gObject->AddComponent(renderer);

		gObject->AddComponent(new FlowTile(type, group, turns));

		if (turns != -1)
		{
			auto text = new TextRenderer();
			text->Text().setFont(ResourceManager::GetFont("font.ttf"));
			text->Text().setColor(sf::Color::Black);
			text->Text().setString(std::to_string(turns));
			text->Text().setCharacterSize(static_cast<unsigned int>(TILE_SIZE / 2));
			text->SetAlignment(TextRenderer::TextAlign::Center);

			gObject->AddComponent(text);
		}

		gObject->Transform()->SetPosition(pos);

		return gObject;

	}

	GameObject* CreateTileDestroyer(sf::Vector2f const& pos)
	{
		GameObject* gObject = new GameObject();
		gObject->SetLayer(Layer::Game);

		gObject->AddComponent(new TileDestroyer());

		auto renderer = new SpriteRenderer("solidGridTile.png");
		renderer->SetSpriteSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		renderer->SetSpriteColor(sf::Color::Red);
		gObject->AddComponent(renderer);

		gObject->Transform()->SetPosition(pos);

		return gObject;
	}

	GameObject* CreateEmptyBoard(int width, int height)
	{
		GameObject* gObject = new GameObject();
		gObject->SetName(NAME_GAME_BOARD);
		gObject->SetTag(TAG_GAME_BOARD);
		gObject->SetLayer(Layer::Game);

		auto mapComp = new BoardMap(width, height);
		gObject->AddComponent(mapComp);

		//Add all the tiles
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				mapComp->CreateEmptyTile(sf::Vector2i(i, j));
			}
		}

		gObject->AddComponent(new InputInteractionComponent());

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

		float size = 32.f;
		sf::Vector2u windowSize = Application::GetWindow().getSize();
		float offset = 20;
		float curHeight = -(windowSize.y - 2 * size) / 2;
		float x = windowSize.x / 2 - 3 * size / 2;
		
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::Single, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::SingleDestroy, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::CornerSmall, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::CornerBig, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::IShapeSmall, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::IShapeBig, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::OShape, 3));
		curHeight += size + offset;
		GameObject::Instantiate(CreatePower(size, sf::Vector2f(x, curHeight), PlaceTilePower::Type::TShape, 3));
		curHeight += size + offset;

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

			if (id == 0)
			{
				board->CreateSolidTile(sf::Vector2i(x, y));
			}
			else if (id <= 9)
			{
			}
		}

		GameObject::Instantiate(grid);

		int flowGroup = 0;
		auto unitsXML = levelNode.child("Entities");
		for (auto& unitXML : unitsXML)
		{
			float x, y;
			float angle;
			auto name = std::string(unitXML.name());

			x = worldTopLeft.x + unitXML.attribute("x").as_float();
			y = worldTopLeft.y + unitXML.attribute("y").as_float();

			angle = unitXML.attribute("angle").as_float();

			if (name == "Flow")
			{
				int type = unitXML.attribute("Type").as_int();
				int turns = unitXML.attribute("Turns").as_int();
				GameObject::Instantiate(CreateFlow(sf::Vector2f(x, y), static_cast<FlowTile::FlowTileType>(type), flowGroup, turns));
				flowGroup++;
			}
		}
	}
}