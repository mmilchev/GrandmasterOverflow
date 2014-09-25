#include "LevelManager.h"
#include "Prefabs.h"
#include "ConfigManager.h"

int LevelManager::sCurrentLevelNum = 0;
std::vector<int> LevelManager::sMessagesShown;

void LevelManager::LoadNextLevel()
{
	if (IsFinalLevel())
		sCurrentLevelNum = 1;
	else
		sCurrentLevelNum++;

	ReloadCurrentLevel();
}

void LevelManager::ReloadCurrentLevel()
{
	if (sCurrentLevelNum < 1)
		return;

	prefabs::LoadLevel("Level-" + std::to_string(sCurrentLevelNum) + ".oel");
}

bool LevelManager::IsFinalLevel()
{
	return sCurrentLevelNum >= ConfigManager::GetInt("[Level Progression]iNumLevels");
}

void LevelManager::LoadLevel(int levelNum)
{
	sCurrentLevelNum = levelNum;
	ReloadCurrentLevel();
}

void LevelManager::LoadPreviousLevel()
{
	sCurrentLevelNum--;
	if (sCurrentLevelNum < 1)
		sCurrentLevelNum = 1;
	ReloadCurrentLevel();
}

void LevelManager::SignalMessagesViewed()
{
	sMessagesShown.push_back(sCurrentLevelNum);
}

bool LevelManager::ShouldShowMessages()
{
	auto found = std::find(sMessagesShown.begin(), sMessagesShown.end(), sCurrentLevelNum);
	return found == sMessagesShown.end();
}
