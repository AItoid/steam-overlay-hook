#include "Includes.h"

BOOL WINAPI DllMain(HMODULE hModule, DWORD attachReason, LPVOID reserved)
{
	if (attachReason == DLL_PROCESS_ATTACH)
	{
		SteamOverlaySetup* steamOverlayHooker = new SteamOverlaySetup();

		// Setup prerequisites.
		if (!steamOverlayHooker->Setup())
			return FALSE;

		// Apply hooks.
		if (!steamOverlayHooker->Hook())
			return FALSE;
	}

	return TRUE;
}