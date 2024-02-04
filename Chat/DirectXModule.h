#pragma once

#define DIRECTX9 DirectXModule::getInstance()

#include <iostream>
#include <d3d9.h>
#include "Singleton.h"

class DirectXModule : public Singleton<DirectXModule> {
private:
    LPDIRECT3D9 _d3d; // Pointer to the DirectX9 interface.
    LPDIRECT3DDEVICE9 _d3ddev; // Pointer to DirectX9 device.
    D3DPRESENT_PARAMETERS _d3dpp; // DirectX9 device parameters.
public:
    ~DirectXModule();
    
    bool Initialize(HWND hWnd);
    void BeginRender();
    void EndRender();
    void Cleanup();

    LPDIRECT3D9 GetDevice();
    LPDIRECT3DDEVICE9 GetDeviceContext();
    D3DPRESENT_PARAMETERS& GetDeviceParameters();
};