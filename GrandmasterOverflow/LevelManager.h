#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <vector>

class LevelManager
{
public:
	static void ReloadCurrentLevel();
	static void LoadNextLevel();
	static void LoadPreviousLevel();
	static void LoadLevel(int levelNum);

	static void SignalMessagesViewed();
	static bool ShouldShowMessages();

	static bool IsFinalLevel();

	static int GetCurrentLevelNum();

	static void InitBgMusic();
private:
	static int sCurrentLevelNum;
	static std::vector<int> sMessagesShown;
};

#endif