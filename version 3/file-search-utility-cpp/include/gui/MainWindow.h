#pragma once

#include "IWindow.h"
#include "ResultsListView.h"
#include "../search/ISearchStrategy.h"
#include <memory>
#include <vector>
#include <string>

/*
 * MainWindow.h — Main Window (SRP + DIP + OCP)
 *
 * SRP : Orchestrates controls and reacts to user actions.
 *       Does NOT implement search or rendering details itself.
 *
 * DIP : Holds ISearchStrategy* — swappable without recompiling.
 *
 * OCP : Adding a new search mode only requires a new
 *       ISearchStrategy subclass; MainWindow stays unchanged.
 */

/* ── Control IDs ─────────────────────────────────────────────── */
constexpr int ID_EDIT_DIR       = 101;
constexpr int ID_EDIT_QUERY     = 102;
constexpr int ID_BTN_BROWSE     = 103;
constexpr int ID_BTN_SEARCH     = 104;
constexpr int ID_BTN_CLEAR      = 105;
constexpr int ID_LISTVIEW       = 106;
constexpr int ID_STATIC_STATUS  = 107;
constexpr int ID_RADIO_NAME     = 108;
constexpr int ID_RADIO_EXT      = 109;
constexpr int ID_RADIO_CONTENT  = 110;
constexpr int ID_STATIC_COUNT   = 111;

/* ── Window size ─────────────────────────────────────────────── */
constexpr int WINDOW_WIDTH  = 820;
constexpr int WINDOW_HEIGHT = 600;

class MainWindow : public IWindow {
public:
    MainWindow();

    // IWindow
    bool create(HINSTANCE hInstance, int nCmdShow) override;
    HWND handle() const override { return hwnd_; }

    // Static WndProc — routes messages to the instance method
    static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

private:
    HWND             hwnd_     = nullptr;
    HINSTANCE        hInst_    = nullptr;
    ResultsListView  listView_;

    // Injected strategy (DIP) — set according to radio selection
    std::unique_ptr<ISearchStrategy> strategy_;

    // All results from the last search
    std::vector<SearchResult> results_;

    /* ── Internal message handlers ───────────────────── */
    LRESULT handleMessage(HWND, UINT, WPARAM, LPARAM);
    void    onCreate(HWND hwnd);
    void    onCommand(WPARAM wParam);

    /* ── Actions ─────────────────────────────────────── */
    void browseFolder();
    void runSearch();
    void clearResults();

    /* ── Helpers ─────────────────────────────────────── */
    ISearchStrategy* buildStrategy() const; // factory based on radio
    void setStatus(const std::string& text);
    void updateCount();
    std::string getDlgText(int ctrlId) const;
};
