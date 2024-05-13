#pragma once
#include "Includes.h"

class SteamOverlaySetup
{
public:
	std::uintptr_t m_gameOverlayRenderer;
	std::uintptr_t* m_presentOriginal;
	std::uintptr_t* m_resizeBuffersOriginal;

public:
	bool Setup();
	bool Hook();
};