#include "offset.h"
#include "mem.h"

HANDLE mem::handle;

unsigned int mem::GetModuleBaseAddress(DWORD pid, const char* module)
{
	unsigned int baseAddr = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);
		if (Module32First(snapshot, &moduleEntry))
		{
			do
			{
				if (!strcmp(moduleEntry.szModule, module))
				{
					baseAddr = (unsigned int)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(snapshot, &moduleEntry));
		}
	}
	CloseHandle(snapshot);
	return baseAddr;
}
