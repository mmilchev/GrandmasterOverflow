#include "LevelManager.h"
#include "Prefabs.h"

int LevelManager::sCurrentLevelNum = 0;

void LevelManager::LoadNextLevel()
{
	sCurrentLevelNum++;
	ReloadCurrentLevel();
}

void LevelManager::ReloadCurrentLevel()
{
	if (sCurrentLevelNum < 1)
		return;

	prefabs::LoadLevel("Level-" + std::to_string(sCurrentLevelNum) + ".oel");
}
