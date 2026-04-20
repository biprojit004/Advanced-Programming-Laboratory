#pragma once

#include <windows.h>
#include <string>

/*
 * IWindow.h — GUI Abstraction (DIP)
 *
 * Dependency Inversion Principle:
 *   main.cpp depends on IWindow — not on MainWindow directly.
 *   A future unit-test or headless mode can swap in a mock.
 */
class IWindow {
public:
    virtual ~IWindow() = default;

    // Create and show the window; returns false on failure
    virtual bool create(HINSTANCE hInstance, int nCmdShow) = 0;

    // Return the underlying HWND (needed for the message loop)
    virtual HWND handle() const = 0;
};
