#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TILE_SIZE 128.f

#define MIN_MESSAGE_LABEL sf::Vector2f(512, 128)
#define MESSAGE_BORDER 32.f

#define TAG_TILE				"Terrain"
#define TAG_FLOW				"Flow"
#define TAG_GAME_BOARD			"GameBoard"
#define TAG_GAME_STATE			"GameState"
#define TAG_GAME_CAMERA			"GameCamera"
#define TAG_GUI_CAMERA			"GuiCamera"
#define TAG_SIDEBAR				"Sidebar"
#define TAG_SLOT				"Slot"
#define TAG_GHOST_POWER			"GhostPower"
#define TAG_POWER_TILE			"PowerTile"
#define TAG_TIME_MENU			"TimeMenu"
#define TAG_PAUSE_BUTTON		"PauseButton"
#define TAG_NORMAL_SPEED_BUTTON	"NSButton"
#define TAG_FAST_FORWARD_BUTTON "FFButton"

#define NAME_GAME_BOARD			"GameBoard"
#define NAME_GAME_STATE			"GameState"
#define NAME_CAMERA				"Camera"
#define NAME_ICON_POWER			"PowerIcon"
#define NAME_TEXT_POWER			"PowerText"
#define NAME_POWER_TILE_CREATE	"TileCreate"
#define NAME_POWER_TILE_DESTROY	"TileDestroy"

enum Layer
{
	Default = 0,
	Game	= 1 << 0,
	GUI		= 1 << 1
};

#endif