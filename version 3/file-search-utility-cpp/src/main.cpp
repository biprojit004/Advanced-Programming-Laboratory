#include "../include/gui/IWindow.h"
#include "../include/gui/MainWindow.h"
#include <windows.h>
#include <memory>

/*
 * main.cpp — Program entry point (SRP + DIP)
 *
 * Single Responsibility : only initializes and runs the message loop.
 * Dependency Inversion  : depends on IWindow*, not MainWindow directly.
 *
 * All GUI logic  → MainWindow / ResultsListView
 * All search logic → ISearchStrategy implementations
 * All helpers    → FileUtils namespace
 */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE /*hPrevInstance*/,
                   LPSTR     /*lpCmdLine*/,
                   int       nCmdShow)
{
    // DIP: program talks to IWindow, not MainWindow
    std::unique_ptr<IWindow> window = std::make_unique<MainWindow>();

    if (!window->create(hInstance, nCmdShow)) {
        MessageBoxA(nullptr,
                    "Failed to create the application window.",
                    "Fatal Error", MB_ICONERROR);
        return 1;
    }

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return static_cast<int>(msg.wParam);
}
