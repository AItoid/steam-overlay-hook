#include "Includes.h"

HMODULE Util::GetModule(std::string moduleName)
{
	// Get PEB data.
#ifdef _WIN64
	PEB_NEW* peb = (PEB_NEW*)__readgsqword(0x60);
#else
	PEB_NEW* peb = (PEB_NEW*)__readfsdword(0x30);
#endif
	if (peb == nullptr)
		return nullptr;

	PLIST_ENTRY listEntry = peb->Ldr->InLoadOrderModuleList.Flink;
	PLOADER_TABLE_ENTRY tableEntry = nullptr;

	// Iterate each module.
	while (listEntry != &peb->Ldr->InLoadOrderModuleList && listEntry)
	{
		// Declare table.
		tableEntry = CONTAINING_RECORD(listEntry, LOADER_TABLE_ENTRY, InLoadOrderLinks);

		std::wstring wideNameString(tableEntry->BaseDllName.Buffer);
		std::string currentIteratedModuleName(wideNameString.begin(), wideNameString.end());

		// Convert string to lowercase.
		std::transform(currentIteratedModuleName.begin(), currentIteratedModuleName.end(), currentIteratedModuleName.begin(), ::tolower);

		HMODULE base = (HMODULE)tableEntry->DllBase;

		if (!strcmp(moduleName.c_str(), currentIteratedModuleName.c_str()))
			return (HMODULE)tableEntry->DllBase;

		// Update flink.
		listEntry = listEntry->Flink;
	}

	return nullptr;
}