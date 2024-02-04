#pragma once

#define UIWINDOW UIWindow::getInstance()
#define UI_WINDOW_CLASSNAME L"UI_WINDOW_EDENTHER"

#include <iostream>
#include <tchar.h>

#include "DirectXModule.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

class UIWindow : public Singleton<UIWindow>
{
private:
	HINSTANCE _hInstance; // Win32 instance handle.
	HWND _hWnd; // Window handle.

	UINT _x;
	UINT _y;
	UINT _width;
	UINT _height;
	UINT _resizeWidth = 0, _resizeHeight = 0;
	
	bool _running = true;
	bool _showChatWindow = true;

	BOOL InitImGui();

	VOID Update();
	VOID Draw();

	VOID ResetDevice();
public:
	UIWindow();
	~UIWindow();

	BOOL Init(UINT x, UINT y, UINT width, UINT height);
	VOID Run();
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

