#include "LevelManager.h"

#include <Application.h>
#include <ConfigManager.h>

//Remove later
#include <iostream>

void LoadConfigs()
{
	ConfigManager::LoadFile("MainConfig.ini");
}

void ConfigModules()
{
	auto& physics = Application::GetPhysicsModule();
	physics.SetTimeStep(ConfigManager::GetFloat("[Physics]fTimeStep"));
	physics.SetVelocityItterations(ConfigManager::GetInt("[Physics]iVelocityItterations"));
	physics.SetPositionItterations(ConfigManager::GetInt("[Physics]iPositionItterations"));
}

int main()
{
	LoadConfigs();

	const auto& windowSize = ConfigManager::GetVector("[Window]vWindowSize");
	bool fullscreen = ConfigManager::GetBool("[Window]bFullscreen");
	sf::Uint32 style = fullscreen?sf::Style::None:sf::Style::Default;
	sf::VideoMode mode = sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	if (fullscreen)
	{
		mode = sf::VideoMode().getDesktopMode();
	}
	sf::ContextSettings settings;
	Application application(mode, ConfigManager::GetString("[Window]sAppName"), style, settings);

	application.Initialize(ConfigManager::GetBool("[Graphics]bRenderToTextures"));

	ConfigModules();

	LevelManager::LoadNextLevel();

	application.Run();

	return 0;
}