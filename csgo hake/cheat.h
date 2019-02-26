#pragma once
#include "mem.h"
#include "offset.h"

//const byte JUMP = 6;

class cheat
{
public:
	static bool isOn;
	static bool once;

	static void handleBhop();
	static void handleAimbot();
	static void handleWallhack();
	static void getTeam();
};