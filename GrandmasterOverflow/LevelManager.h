#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class LevelManager
{
public:
	static void ReloadCurrentLevel();
	static void LoadNextLevel();
	static void LoadPreviousLevel();
	static void LoadLevel(int levelNum);

	static bool IsFinalLevel();

private:
	static int sCurrentLevelNum;
};

#endif