#pragma once
#include "mem.h"
#include <deque>

struct offset
{
	// +jump offset
	const static unsigned int JUMPOFFSET = 0x517E1C4;

	// glowObjectManager
	const static unsigned int GLOWOBJMANAGER = 0x521AF50;

	// localPlayer
	const static unsigned int LPOFFSET = 0xCCA6A4;
	const static unsigned int LP_STATUSOFFSET = 0x104;
	const static unsigned int LP_AIMPUNCHPITCH = 0x302C;

	// entityList
	const static unsigned int ELOFFSET = 0x4CDB00C;

	// baseClientState
	const static unsigned int BCSOFFSET = 0x58BCFC;
	const static unsigned int BCS_PITCH = 0x4D88;
	const static unsigned int BCS_YAW = 0x4D8C;

	// bone matrix
	const static unsigned int BMOFFSET = 0x26A8;
	const static unsigned int BM_HEAD_X = 0x18C;
	const static int BM_COORDOFFSET = 0x10;

	// global vars
	const static unsigned int INTERVALBETWEENTICK = 0x58BA00 + 0x20;

	struct flag
	{
		const static unsigned char JUMP = 6;
	};

	struct modules
	{
		static unsigned int panoramadll;
		static unsigned int enginedll;
	};

	struct dynamic
	{
		// LPOFFSET
		static unsigned int localPlayer;
		
		// BCSOFFSET
		static unsigned int baseClientState;

		// JUMPOFFSET
		static void* localJumpAddr;

		// GLOWOBJECTMANAGER
		static unsigned int glowObjectManager;

		// LP_STATUSOFFSET
		static void* localStatusAddr;

		//ELOFFSET
		static unsigned int entityListBase;
		static std::deque<unsigned int> entityList;
		static std::deque<int> entityTeamMap;

		// global vars
		static float intervalBetweenTick;

	};

	struct player
	{
		const static unsigned int eyeZOffset = 0x110;

		const static unsigned int velocityX = 0x114;

		const static unsigned int vecOriginX = 0x138;

		const static unsigned int health = 0x100;

		const static unsigned int glowIndex = 0xA3F8;

		const static unsigned int teamNum = 0xF4;

	};

	struct glowObject
	{
		const static unsigned int r = 0x4;
		const static unsigned int trueflag = 0x24;
	};
};
