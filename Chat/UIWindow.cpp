#include "UIWindow.h"

namespace { UIWindow* ui = 0; }

UIWindow::UIWindow() : _x(0), _y(0), _width(0), _height(0), _hInstance(0), _hWnd(0), _running(true), _showChatWindow(true)
{
    ui = this;
}

UIWindow::~UIWindow()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DestroyWindow(_hWnd);
    ::UnregisterClassW(UI_WINDOW_CLASSNAME, _hInstance);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return ui->WindowProc(hWnd, message, wParam, lParam);
}

BOOL UIWindow::InitImGui()
{
    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls.
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls.

    // Setup Dear ImGui style.
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends.
    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX9_Init(DIRECTX9.GetDeviceContext());

    return true;
}

VOID UIWindow::Update() { if (!_showChatWindow) exit(0); }

VOID UIWindow::Draw()
{
    // Handle window resize (we don't resize directly in the WM_SIZE handler).
    if (_resizeWidth != 0 && _resizeHeight != 0)
    {
        DIRECTX9.GetDeviceParameters().BackBufferWidth = _resizeWidth;
        DIRECTX9.GetDeviceParameters().BackBufferHeight = _resizeHeight;
        _resizeWidth = _resizeHeight = 0;
        ResetDevice();
    }

    // Start the Dear ImGui frame.
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    {
        // Get a reference to the ImGui style structure.
        ImGuiStyle& style = ImGui::GetStyle();

        // Sets the main window semi transparent.
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));

        // Begin a new ImGui window for the chat.
        ImGui::Begin("Edenther (Chat Box)", &_showChatWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

        // Chat window parameters (defaults 600x256).
        ImGui::SetWindowSize(ImVec2(828, 200));

        // Pushes styles for the child window.
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 44, 64, 128));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));

        // Display area for chat messages
        // Use of a child window to contain messages and make it scrollable (window can still scroll with the flag ImGuiWindowFlags_NoScrollbar but scrollbar hidden).
        ImGui::BeginChild("Messages", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_NoScrollbar);

        // Iteration over the messages would go here.
        // For example: ImGui::Text("Message from user");

        ImGui::Text("Player 1 : Are we done yet?");
        ImGui::Text("Player 2 : Oh boi, now comes the networking part...");

        ImGui::EndChild();

        // End stacks for child window.
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        // Input text field (buffer can be adjusted).
        static char messageBuffer[512] = "";
        // ImGui::InputText("Message", messageBuffer, IM_ARRAYSIZE(messageBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::InputTextWithHint("##Message", "Write your message here you doughnut!", messageBuffer, IM_ARRAYSIZE(messageBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

        // On the same line with a margin.
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 0.5f);

        // Submit.
        if (ImGui::Button("Send", ImVec2(129, 19))) {
            // More code to handle the message goes here and clears buffer.
            messageBuffer[0] = '\0';
        }

        // Quit button (on the same line with a margin).
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 0.5f);

        if (ImGui::Button("Quit", ImVec2(128, 19))) _showChatWindow = false;

        ImGui::End();

        ImGui::PopStyleColor();
    }

    // Rendering
    ImGui::EndFrame();

    // Should we call from here on BeginRender and EndRender from DirectXModule class here?
    // See below.

    DIRECTX9.GetDeviceContext()->SetRenderState(D3DRS_ZENABLE, FALSE);
    DIRECTX9.GetDeviceContext()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    DIRECTX9.GetDeviceContext()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    DIRECTX9.GetDeviceContext()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    if (DIRECTX9.GetDeviceContext()->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        DIRECTX9.GetDeviceContext()->EndScene();
    }

    HRESULT result = DIRECTX9.GetDeviceContext()->Present(nullptr, nullptr, nullptr, nullptr);

    // Handle loss of D3D9 device.
    if (result == D3DERR_DEVICELOST && DIRECTX9.GetDeviceContext()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();
}

VOID UIWindow::ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = DIRECTX9.GetDeviceContext()->Reset(&DIRECTX9.GetDeviceParameters());
    
    if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
    
    ImGui_ImplDX9_CreateDeviceObjects();
}

VOID UIWindow::Run()
{
    // Il faudrait expliquer cela.
    MSG msg = { 0 };

    // Attends le prochain message dans la file d'attente et stocke le résultat dans msg.
    while (msg.message != WM_QUIT)
    {
        // Vérifie tous les messages en attente.
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            // Traduit et transfert le message vers WindowProc().
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT) _running = false;
        }

        if (!_running) break;

        Update();
        Draw();
    }
}

BOOL UIWindow::Init(UINT x, UINT y, UINT width, UINT height)
{
    RECT rc;
    GetWindowRect(GetDesktopWindow(), &rc);
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXMAXTRACK); // If you have more than one monitor, you will get the total width.
    rc.bottom = GetSystemMetrics(SM_CYMAXTRACK);

    WNDCLASSEX wc;

    // Nettoie le bloc de mémoire correspondant à la classe.
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindowProc;
    wc.cbClsExtra = 0; // Extra.
    wc.cbWndExtra = 0; // Extra.
    wc.hInstance = _hInstance;
    wc.hIcon = NULL; // Icône de la fenêtre.
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL; // Extra.
    wc.lpszClassName = UI_WINDOW_CLASSNAME;
    wc.hIconSm = NULL; // Extra.

    ::RegisterClassEx(&wc);

    _hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, UI_WINDOW_CLASSNAME, L"Edenther", WS_POPUP, 0, 0, rc.right + 500, rc.bottom, NULL, NULL, _hInstance, NULL);

    SetLayeredWindowAttributes(_hWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);

    // Si l'initialisation de DirectX a échoué.
    if (!DIRECTX9.Initialize(_hWnd))
    {
        DIRECTX9.Cleanup();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window.
    ::ShowWindow(_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(_hWnd);

    SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    // Initialisation ImGui.
    if (!InitImGui()) return false;

    return true;
}

// Forward declare message handler from imgui_impl_win32.cpp.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK UIWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return true;

    switch (message)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) return 0;
        
        // Queue resize.
        _resizeWidth = (UINT)LOWORD(lParam);
        _resizeHeight = (UINT)HIWORD(lParam);

        return 0;
    case WM_SYSCOMMAND:
        // Disable ALT application menu.
        if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    
    return ::DefWindowProcW(hWnd, message, wParam, lParam);
}
