#include "LevelManager.h"
#include "Prefabs.h"
#include "ConfigManager.h"

int LevelManager::sCurrentLevelNum = 0;

void LevelManager::LoadNextLevel()
{
	if (LastLevel())
		return;

	sCurrentLevelNum++;
	ReloadCurrentLevel();
}

void LevelManager::ReloadCurrentLevel()
{
	if (sCurrentLevelNum < 1)
		return;

	prefabs::LoadLevel("Level-" + std::to_string(sCurrentLevelNum) + ".oel");
}

bool LevelManager::LastLevel()
{
	return sCurrentLevelNum >= ConfigManager::GetInt("[Level Progression]iNumLevels");
}
