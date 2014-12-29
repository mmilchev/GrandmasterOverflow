#include "MusicController.h"
#include <ResourceManager.h>
#include <ConfigManager.h>


MusicController::MusicController()
{
}

void MusicController::Start()
{
	sf::Listener::setGlobalVolume(ConfigManager::GetFloat("[Audio]fMasterVolume"));
	m_Music.openFromFile(ResourceManager::ksResourceFolder + ResourceManager::ksMusicFolder + "background.ogg");
	m_Music.setLoop(true);
	m_Music.play();
}
