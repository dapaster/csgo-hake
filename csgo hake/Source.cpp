#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <deque>
#include <thread>
#include "offset.h"
#include "cheat.h"
#include "mem.h"


void moduleSetup();
void setup();

DWORD pid;

int main()
{
	HWND wnd = FindWindowA(0, "Counter-Strike: Global Offensive");

	if (!wnd)
	{
		std::cout << "Could not find window";
		return 0;
	}

	GetWindowThreadProcessId(wnd, &pid);
	HANDLE handle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);
	mem::handle = handle;

	if (!mem::handle)
	{
		std::cout << "Hook failed";
		return 0;
	}


	std::cout << "Hooked\n";

	moduleSetup();
	setup();
	
	std::cout << "Setup Complete, running\n";


	while (true)
	{
		while (GetForegroundWindow() != wnd)
		{
			cheat::isOn = false;
			cheat::once = true;
			Sleep(1000);
		}

		cheat::isOn = true;

		cheat::handleBhop();


		if (cheat::once)
		{
			#define thread(x) thread([]{while(cheat::isOn){x();Sleep(1);}})

			std::thread(cheat::handleBhop).detach();
			std::thread(cheat::handleWallhack).detach();
			std::thread(cheat::handleAimbot).detach();

			#undef thread

			cheat::once = false;
		}


	}

	return 1;
}

void moduleSetup()
{
	offset::modules::panoramadll = mem::GetModuleBaseAddress(pid, "client_panorama.dll");
	offset::modules::enginedll = mem::GetModuleBaseAddress(pid, "engine.dll");

}

void setup()
{
	offset::dynamic::localJumpAddr = (void*)(offset::modules::panoramadll + offset::JUMPOFFSET);
	offset::dynamic::baseClientState = mem::read<unsigned int>((void*)(offset::modules::enginedll + offset::BCSOFFSET));

	offset::dynamic::localPlayer = mem::read<unsigned int>((void*)(offset::modules::panoramadll + offset::LPOFFSET));
	offset::dynamic::localStatusAddr = (void*)(offset::dynamic::localPlayer + offset::LP_STATUSOFFSET);
	
	offset::dynamic::intervalBetweenTick = mem::read<float>((void*)(offset::modules::enginedll + offset::INTERVALBETWEENTICK));

	offset::dynamic::entityListBase = offset::modules::panoramadll + offset::ELOFFSET;

	offset::dynamic::glowObjectManager = mem::read<unsigned int>((void*)(offset::modules::panoramadll + offset::GLOWOBJMANAGER));

	cheat::getTeam();

	// get all players in server
	for (int i = 1;; i++)
	{
		unsigned int now = mem::read<unsigned int>((void*)(offset::dynamic::entityListBase + sizeof(int) * 4 * i));

		if (now == 0)
		{
			if (i != 1)
				break;
		}
		else
		{
			//std::cout << "\n" << std::hex << now << "\n";
			offset::dynamic::entityList.push_back(now);
			offset::dynamic::entityTeamMap.push_back(mem::read<int>((void*)(now + offset::player::teamNum)));
		}
	}

}