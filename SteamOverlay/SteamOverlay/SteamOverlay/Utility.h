#pragma once
#include "Includes.h"

typedef struct PEB_LOADER_DATA
{
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LOADER_DATA, * PPEB_LOADER_DATA;

typedef struct PEB_NEW
{
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LOADER_DATA* Ldr;
} PEB_NEW, * PPEB_NEW;

typedef struct _UNICODE_STRINGG
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRINGG;

typedef struct LOADER_TABLE_ENTRY
{
	LIST_ENTRY				InLoadOrderLinks;
	LIST_ENTRY				InMemoryOrderLinks;
	LIST_ENTRY				InInitializationOrderLinks;
	uintptr_t				DllBase;
	uintptr_t				EntryPoint;
	uint32_t				SizeOfImage;
	UNICODE_STRINGG			FullDllName;
	UNICODE_STRINGG			BaseDllName;
	uint8_t					FlagGroup[4];
	uint32_t				Flags;
	uint16_t				ObsoleteLoadCount;
	uint16_t				TlsIndex;
	LIST_ENTRY				HashLinks;
	uint32_t				TimeDateStamp;
	uintptr_t				EntryPointActivationContext;
	uintptr_t				Lock;
	uintptr_t				DdagNode;
	LIST_ENTRY				NodeModuleLink;
	uintptr_t				LoadContext;
	uintptr_t				ParentDllBase;
} LOADER_TABLE_ENTRY, * PLOADER_TABLE_ENTRY;

namespace Util
{
	HMODULE GetModule(std::string moduleName);
}