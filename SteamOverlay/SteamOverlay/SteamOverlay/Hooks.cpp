#include "Includes.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	return CallWindowProcA(Hooks::ImGuiData::oWndProc, hWnd, msg, wParam, lParam);
}

HRESULT __stdcall Hooks::PresentHook(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
	static bool isSetup = false;
	if (!isSetup)
	{
		if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Hooks::ImGuiData::d3d11Device)))
		{
			Hooks::ImGuiData::d3d11Device->GetImmediateContext(&Hooks::ImGuiData::deviceContext);

			DXGI_SWAP_CHAIN_DESC sd;
			swapChain->GetDesc(&sd);

			Hooks::ImGuiData::window = sd.OutputWindow;

			ID3D11Texture2D* backBuffer = nullptr;
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
			Hooks::ImGuiData::d3d11Device->CreateRenderTargetView(backBuffer, nullptr, &Hooks::ImGuiData::deviceRenderTargetView);
			backBuffer->Release();

			Hooks::ImGuiData::oWndProc = (WNDPROC)SetWindowLongPtr(Hooks::ImGuiData::window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			io.IniFilename = NULL;
			ImGui_ImplWin32_Init(Hooks::ImGuiData::window);
			ImGui_ImplDX11_Init(Hooks::ImGuiData::d3d11Device, Hooks::ImGuiData::deviceContext);

			isSetup = true;
		}
		else
		{
			return oPresent(swapChain, syncInterval, flags);
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();

	Hooks::ImGuiData::deviceContext->OMSetRenderTargets(1, &Hooks::ImGuiData::deviceRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(swapChain, syncInterval, flags);
}

HRESULT __stdcall Hooks::ResizeBuffersHook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
{
	if (Hooks::ImGuiData::deviceRenderTargetView)
	{
		Hooks::ImGuiData::deviceContext->OMSetRenderTargets(0, 0, 0);
		Hooks::ImGuiData::deviceRenderTargetView->Release();
	}

	HRESULT result = oResizeBuffers(swapChain, bufferCount, width, height, newFormat, swapChainFlags);

	ID3D11Texture2D* buffer = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	Hooks::ImGuiData::d3d11Device->CreateRenderTargetView(buffer, NULL, &Hooks::ImGuiData::deviceRenderTargetView);
	buffer->Release();

	Hooks::ImGuiData::deviceContext->OMSetRenderTargets(1, &Hooks::ImGuiData::deviceRenderTargetView, NULL);

	D3D11_VIEWPORT vp{};
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	Hooks::ImGuiData::deviceContext->RSSetViewports(1, &vp);
	return result;
}