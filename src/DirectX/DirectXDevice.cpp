#include "pch.h"

#include <d3d11.h>

#include "DirectXDevice.h"

DirectXDevice::DirectXDevice(const UniquePtr<Window>& window)
	: m_Device(nullptr), m_DeviceContext(nullptr),
	m_MainRenderTargetView(nullptr), m_SwapChain(nullptr)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->GetNativeWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevelArray, 2, D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, &featureLevel, &m_DeviceContext) != S_OK)
		FatalError();

	CreateRenderTarget();

	window->OnProcedure.Subscribe([this](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_SIZE && m_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			m_SwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
	});
}

DirectXDevice::~DirectXDevice()
{
	CleanupRenderTarget();
	if (m_SwapChain) { m_SwapChain->Release(); m_SwapChain = NULL; }
	if (m_DeviceContext) { m_DeviceContext->Release(); m_DeviceContext = NULL; }
	if (m_Device) { m_Device->Release(); m_Device = NULL; }
}

void DirectXDevice::CleanupRenderTarget()
{
	if (m_MainRenderTargetView) { m_MainRenderTargetView->Release(); m_MainRenderTargetView = NULL; }
}

void DirectXDevice::CreateRenderTarget()
{
	ID3D11Texture2D* backBuffer;
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (!backBuffer)
		return;

	m_Device->CreateRenderTargetView(backBuffer, NULL, &m_MainRenderTargetView);
	backBuffer->Release();
}

void DirectXDevice::SetRenderTarget()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_MainRenderTargetView, NULL);
}

void DirectXDevice::Present()
{
	m_SwapChain->Present(1, 0);
}
