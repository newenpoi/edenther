#include "DirectXModule.h"

LPDIRECT3D9 DirectXModule::GetDevice() { return _d3d; }

LPDIRECT3DDEVICE9 DirectXModule::GetDeviceContext() { return _d3ddev; }

D3DPRESENT_PARAMETERS& DirectXModule::GetDeviceParameters() { return _d3dpp; }

DirectXModule::~DirectXModule()
{
    Cleanup();
}

bool DirectXModule::Initialize(HWND hWnd)
{
    if ((_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr) return false;

    // Creates the d3d device.
    ZeroMemory(&_d3dpp, sizeof(_d3dpp));
    
    // Parameters.
    _d3dpp.Windowed = TRUE;
    _d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    _d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    _d3dpp.EnableAutoDepthStencil = TRUE;
    _d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    _d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync.

    // Returns false if the d3d device crashed or has not been ititialized properly.
    if (_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &_d3dpp, &_d3ddev) < 0) return false;

    return true;
}

void DirectXModule::Cleanup()
{
    if (_d3ddev) { _d3ddev->Release(); _d3ddev = nullptr; }
    if (_d3d) { _d3d->Release(); _d3d = nullptr; }
}

void DirectXModule::BeginRender()
{
}

void DirectXModule::EndRender()
{
}
