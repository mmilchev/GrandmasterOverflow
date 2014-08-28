#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SOLDIER_SPRITE_SIZE 64.f
#define BULLET_SIZE 16.f
#define TILE_SIZE 128.f
#define GRID_WALL_WIDTH 8.f

#define TAG_UNIT			"BoardUnit"
#define TAG_TILE			"Terrain"
#define TAG_GAME_BOARD		"GameBoard"
#define TAG_GAME_STATE		"GameState"
#define TAG_TEAM			"Team"
#define TAG_BULLET			"Bullet"
#define TAG_RAYCAST_SPOT	"RaycastSpot"
#define TAG_UNIT_GLOW		"UnitGlow"
#define TAG_GAME_CAMERA		"GameCamera"
#define TAG_GUI_CAMERA		"GuiCamera"
#define TAG_SIDEBAR			"Sidebar"
#define TAG_SLOT			"Slot"
#define TAG_GHOST_POWER		"GhostPower"

#define NAME_TEAM		"Team"
#define NAME_GAME_BOARD "GameBoard"
#define NAME_GAME_STATE "GameState"
#define NAME_CAMERA		"Camera"

enum Layer
{
	Default = 0,
	Game	= 1 << 0,
	GUI		= 1 << 1
};

#endif