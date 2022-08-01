#pragma once

#include "Core/Layer.h"
#include "Core/Window.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
class DirectXDevice
{
public:
	DirectXDevice(const UniquePtr<Window>& window);
	~DirectXDevice();

	void CleanupRenderTarget();
	void CreateRenderTarget();
	void SetRenderTarget();
	void Present();

	inline ID3D11Device* GetNativeDevice() { return m_Device; }
	inline ID3D11DeviceContext* GetNativeDeviceContext() { return m_DeviceContext; }

private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_MainRenderTargetView;
};
