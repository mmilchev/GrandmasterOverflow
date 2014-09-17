#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class LevelManager
{
public:
	static void ReloadCurrentLevel();
	static void LoadNextLevel();

	static bool LastLevel();

private:
	static int sCurrentLevelNum;
};

#endif