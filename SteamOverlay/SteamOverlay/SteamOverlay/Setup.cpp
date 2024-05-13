#include "Includes.h"

bool SteamOverlaySetup::Setup()
{
	static std::uintptr_t module64 = reinterpret_cast<std::uintptr_t>(Util::GetModule("gameoverlayrenderer64.dll"));
	static std::uintptr_t module32 = reinterpret_cast<std::uintptr_t>(Util::GetModule("gameoverlayrenderer.dll"));

	if (module64)
	{
		m_gameOverlayRenderer = module64;
	}
	else if (module32)
	{
		m_gameOverlayRenderer = module32;
	}
	else
	{
		m_gameOverlayRenderer = NULL;
	}

	if (!m_gameOverlayRenderer)
		return false;

	// XREF: "Hooking vtable for swap chain\n"
	if (m_gameOverlayRenderer == module64)
	{
		m_presentOriginal = reinterpret_cast<std::uintptr_t*>(m_gameOverlayRenderer + 0x14BBB0);
		m_resizeBuffersOriginal = reinterpret_cast<std::uintptr_t*>(m_gameOverlayRenderer + 0x14BBB8);
	}
	else if (m_gameOverlayRenderer == module32)
	{
		m_presentOriginal = reinterpret_cast<std::uintptr_t*>(m_gameOverlayRenderer + 0x111608);
		m_resizeBuffersOriginal = reinterpret_cast<std::uintptr_t*>(m_gameOverlayRenderer + 0x11160C);
	}

	if (!m_presentOriginal || !m_resizeBuffersOriginal)
		return false;

	return true;
}

bool SteamOverlaySetup::Hook()
{
	// Store originals.
	Hooks::oPresent = reinterpret_cast<Hooks::Present_t>(*m_presentOriginal);
	Hooks::oResizeBuffers = reinterpret_cast<Hooks::ResizeBuffers_t>(*m_resizeBuffersOriginal);

	// Redirect.
	*reinterpret_cast<std::uintptr_t**>(m_presentOriginal) = reinterpret_cast<std::uintptr_t*>(&Hooks::PresentHook);
	*reinterpret_cast<std::uintptr_t**>(m_resizeBuffersOriginal) = reinterpret_cast<std::uintptr_t*>(&Hooks::ResizeBuffersHook);

	return true;
}