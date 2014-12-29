#include "LevelManager.h"

#include <Application.h>
#include <ConfigManager.h>

void LoadConfigs()
{
	ConfigManager::LoadFile("MainConfig.ini");
}

int main()
{
	LoadConfigs();

	//Initialize application
	const auto& windowSize = ConfigManager::GetVector("[Window]vWindowSize");
	bool fullscreen = ConfigManager::GetBool("[Window]bFullscreen");

	sf::Uint32 style;
	sf::VideoMode mode;
	if (fullscreen)
	{
		//Get desktop resolution
		mode = sf::VideoMode().getDesktopMode();
		style = sf::Style::None;
	}
	else
	{
		mode = sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
		style = sf::Style::Titlebar | sf::Style::Close;
	}
	sf::ContextSettings settings;
	Application application(mode, ConfigManager::GetString("[Window]sAppName"), style, settings);

	application.Initialize(ConfigManager::GetBool("[Graphics]bRenderToTextures"));

	//Load game objects in scene
	LevelManager::LoadLevel(ConfigManager::GetInt("[Level Progression]iStartLevel"));
	LevelManager::InitBgMusic();

	//Run game loop
	application.Run();

	return 0;
}