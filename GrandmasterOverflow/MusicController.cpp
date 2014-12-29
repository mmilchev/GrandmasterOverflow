#include "MusicController.h"
#include <ResourceManager.h>


MusicController::MusicController()
{
}

void MusicController::Start()
{
	m_Music.openFromFile(ResourceManager::ksResourceFolder + ResourceManager::ksMusicFolder + "background.ogg");
	m_Music.setLoop(true);
	m_Music.play();
}
