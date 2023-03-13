#pragma once

#define PALETTE_WIDTH		300
#define BOARD_BOX_SIZE		40

enum class tMenuEvent
{
	Default,
	Blocked,
	Spawn_Character,
	Spawn_Monster
};

enum class eType
{
	Tile,
	Block,
	Character,
	None
};

typedef struct _boardData
{
	int idx;
	eType type;
} tBoardData;