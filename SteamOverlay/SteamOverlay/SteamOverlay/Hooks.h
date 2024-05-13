#pragma once
#include "Includes.h"

namespace Hooks
{
	namespace ImGuiData
	{
		inline HWND window = nullptr;
		inline WNDPROC oWndProc = nullptr;
		inline ID3D11Device* d3d11Device = nullptr;
		inline ID3D11DeviceContext* deviceContext = nullptr;
		inline ID3D11RenderTargetView* deviceRenderTargetView = nullptr;
	}

	using Present_t = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
	using ResizeBuffers_t = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

	inline Present_t oPresent = nullptr;
	inline ResizeBuffers_t oResizeBuffers = nullptr;

	HRESULT __stdcall PresentHook(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
	HRESULT __stdcall ResizeBuffersHook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
}