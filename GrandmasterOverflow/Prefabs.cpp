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
#include "ButtonBehaviour.h"
#include "ScreenIconAnimation.h"
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
#include <sigslot.h>

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

	std::map<std::string, PlaceTilePower::Type> kPowerNames =
	{
		{ "Single", PlaceTilePower::Type::Single },
		{ "SingleDestroy", PlaceTilePower::Type::SingleDestroy },
		{ "CornerSmall", PlaceTilePower::Type::CornerSmall },
		{ "CornerBig", PlaceTilePower::Type::CornerBig },
		{ "IShapeSmall", PlaceTilePower::Type::IShapeSmall },
		{ "IShapeBig", PlaceTilePower::Type::IShapeBig },
		{ "TShape", PlaceTilePower::Type::TShape },
		{ "OShape", PlaceTilePower::Type::OShape },
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

	GameObject* CreatePower(float size, sf::Vector2f const& pos, std::string const& textureName, int uses)
	{
		GameObject* gObject = new GameObject();
		gObject->SetTag(TAG_SLOT);
		gObject->SetLayer(Layer::GUI);

		auto innerIcon = new GameObject();
		innerIcon->SetName(NAME_ICON_POWER);
		innerIcon->SetTag(TAG_SLOT);
		innerIcon->SetLayer(Layer::GUI);

		auto renderer = new SpriteRenderer(textureName);
		renderer->SetSpriteSize(sf::Vector2f(size, size));
		innerIcon->AddComponent(renderer);

		innerIcon->SetParent(gObject);
		innerIcon->Transform()->SetLocalPosition(sf::Vector2f(-size / 2, 0));

		auto innerText = new GameObject();
		innerText->SetName(NAME_TEXT_POWER);
		innerText->SetTag(TAG_SLOT);
		innerText->SetLayer(Layer::GUI);

		auto textRenderer = new TextRenderer();
		textRenderer->Text().setFont(ResourceManager::GetFont("font.ttf"));
		textRenderer->Text().setCharacterSize(static_cast<int>(3 * size / 5));
		textRenderer->Text().setColor(sf::Color::White);
		textRenderer->Text().setString("x" + std::to_string(uses));
		textRenderer->SetAlignment(TextRenderer::Center);
		innerText->AddComponent(textRenderer);

		innerText->SetParent(gObject);
		innerText->Transform()->SetLocalPosition(sf::Vector2f(size / 2, 0));

		int intSize = static_cast<int>(size);
		gObject->AddComponent(new BoxInteractionComponent(2 * intSize, intSize));

		gObject->AddComponent(new InputInteractionComponent());
		gObject->AddComponent(new ButtonBehaviour());

		gObject->Transform()->SetPosition(pos);

		return gObject;
	}

	GameObject* CreatePlaceTilePower(float size, sf::Vector2f const& pos, PlaceTilePower::Type type, int uses)
	{
		auto gObject = CreatePower(size, pos, kPowerTextures[type], uses);

		auto power = new PlaceTilePower(type, uses);
		gObject->AddComponent(power);

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

		pugi::xml_node levelNode = doc.child("level");

		float size = 32.f;
		sf::Vector2u windowSize = Application::GetWindow().getSize();
		float offset = 20;
		float curHeight = -(windowSize.y - 2 * size) / 2 + 2 * offset;
		float x = windowSize.x / 2 - 3 * size / 2;

		GameObject::Instantiate(CreateTimeMenu(size));

		//Separate global from tile placing powers
		curHeight += 2 * offset;

		for (auto& attribute : levelNode.attributes())
		{
			auto name = attribute.name();
			if (kPowerNames.find(name) != kPowerNames.end())
			{
				int value = attribute.as_int();
				if (value != 0)
				{
					GameObject::Instantiate(CreatePlaceTilePower(size, sf::Vector2f(x, curHeight), kPowerNames[name], value));
					curHeight += size + offset;
				}
			}
		}

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

				auto flow = CreateFlow(sf::Vector2f(x, y), static_cast<FlowTile::FlowTileType>(type), flowGroup, turns);
				flow->GetComponent<FlowTile>()->SetShouldScale(false);
				GameObject::Instantiate(flow);

				flowGroup++;
			}
		}
	}

	GameObject* CreateIconAnimation(std::string const& textureName)
	{
		auto gObject = new GameObject();
		gObject->SetLayer(Layer::GUI);

		auto wSize = Application::GetWindow().getSize();
		gObject->AddComponent(new BoxInteractionComponent(wSize.x, wSize.y));

		auto renderer = new SpriteRenderer(textureName);
		renderer->SetSpriteColor(sf::Color(170, 170, 170, 255));
		gObject->AddComponent(renderer);

		gObject->AddComponent(new ScreenIconAnimation());

		return gObject;
	}

	GameObject* CreateTimeMenu(float height)
	{
		auto gObject = new GameObject();
		gObject->SetTag(TAG_TIME_MENU);
		gObject->SetLayer(Layer::GUI);

		const int numElements = 3;
		auto wSize = Application::GetWindow().getSize();
		float border = height / 4;
		gObject->Transform()->SetPosition(sf::Vector2f(wSize.x/2 - numElements * (border + height) / 2 , -(wSize.y/2 - border - height / 2)));

		float offset = (wSize.x/2 - border - height/2) - gObject->Transform()->Position().x;

		{
			auto fastForwardObject = new GameObject();
			fastForwardObject->SetLayer(Layer::GUI);

			auto renderer = new SpriteRenderer("fastForwardIcon.png");
			renderer->SetSpriteSize(sf::Vector2f(height, height));
			fastForwardObject->AddComponent(renderer);

			fastForwardObject->AddComponent(new InputInteractionComponent());
			fastForwardObject->AddComponent(new FastforwardButtonBehaviour());

			fastForwardObject->SetParent(gObject);

			fastForwardObject->Transform()->SetLocalPosition(sf::Vector2f(offset, 0));
			offset -= height + border;
		}
		
		{
			auto normalSpeedObject = new GameObject();
			normalSpeedObject->SetLayer(Layer::GUI);

			auto renderer = new SpriteRenderer("normalSpeedIcon.png");
			renderer->SetSpriteSize(sf::Vector2f(height, height));
			normalSpeedObject->AddComponent(renderer);

			normalSpeedObject->AddComponent(new InputInteractionComponent());
			normalSpeedObject->AddComponent(new NormalSpeedButtonBehaviour());

			normalSpeedObject->SetParent(gObject);

			normalSpeedObject->Transform()->SetLocalPosition(sf::Vector2f(offset, 0));
			offset -= height + border;
		}

		{
			auto pauseGameObject = new GameObject();
			pauseGameObject->SetLayer(Layer::GUI);

			auto renderer = new SpriteRenderer("pauseIcon.png");
			renderer->SetSpriteSize(sf::Vector2f(height, height));
			pauseGameObject->AddComponent(renderer);

			pauseGameObject->AddComponent(new InputInteractionComponent());
			pauseGameObject->AddComponent(new PauseButtonBehaviour());

			pauseGameObject->SetParent(gObject);

			pauseGameObject->Transform()->SetLocalPosition(sf::Vector2f(offset, 0));
			offset -= height + border;
		}

		return gObject;
	}

}