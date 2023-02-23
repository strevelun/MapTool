#pragma once

#define PALETTE_WIDTH		300
#define BOARD_BOX_SIZE		40

enum class MenuEvent
{
	Default,
	Blocked,
	Spawn_Character,
	Spawn_Monster
};

enum class Type
{
	Tile,
	Block,
	Character,
	None
};

enum class CharacterType
{
	Player,
	Monster
};