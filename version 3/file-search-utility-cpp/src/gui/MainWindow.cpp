#include "../../include/gui/MainWindow.h"
#include "../../include/search/FileNameSearchStrategy.h"
#include "../../include/search/ExtensionSearchStrategy.h"
#include "../../include/search/ContentSearchStrategy.h"
#include <commctrl.h>
#include <shlobj.h>
#include <string>
#include <sstream>

/* ─────────────────────────────────────────────────────────────
   Constructor
──────────────────────────────────────────────────────────────── */
MainWindow::MainWindow() = default;

/* ─────────────────────────────────────────────────────────────
   create — register class, create window
──────────────────────────────────────────────────────────────── */
bool MainWindow::create(HINSTANCE hInstance, int nCmdShow) {
    hInst_ = hInstance;

    INITCOMMONCONTROLSEX icex{ sizeof(icex), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icex);

    WNDCLASSA wc{};
    wc.lpfnWndProc   = MainWindow::wndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "FileSearchCpp";
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    if (!RegisterClassA(&wc)) return false;

    hwnd_ = CreateWindowExA(
        0, "FileSearchCpp",
        "File Search Utility  [C++ / SOLID]",
        (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance,
        this   // pass 'this' so WndProc can retrieve it
    );

    if (!hwnd_) return false;
    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);
    return true;
}

/* ─────────────────────────────────────────────────────────────
   Static WndProc — routes to instance method
──────────────────────────────────────────────────────────────── */
LRESULT CALLBACK MainWindow::wndProc(HWND hwnd, UINT msg,
                                      WPARAM wParam, LPARAM lParam)
{
    MainWindow* self = nullptr;

    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTA*>(lParam);
        self = reinterpret_cast<MainWindow*>(cs->lpCreateParams);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA,
                          reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<MainWindow*>(
            GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    }

    if (self) return self->handleMessage(hwnd, msg, wParam, lParam);
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

/* ─────────────────────────────────────────────────────────────
   handleMessage
──────────────────────────────────────────────────────────────── */
LRESULT MainWindow::handleMessage(HWND hwnd, UINT msg,
                                   WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
            onCreate(hwnd);
            return 0;

        case WM_COMMAND:
            onCommand(wParam);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

/* ─────────────────────────────────────────────────────────────
   onCreate — build all child controls
──────────────────────────────────────────────────────────────── */
void MainWindow::onCreate(HWND hwnd) {
    hwnd_ = hwnd;

    auto make = [&](const char* cls, const char* txt,
                    DWORD style, int x, int y, int w, int h, int id) {
        return CreateWindowA(cls, txt, WS_CHILD | WS_VISIBLE | style,
                             x, y, w, h, hwnd,
                             reinterpret_cast<HMENU>((LONG_PTR)id),
                             hInst_, nullptr);
    };

    /* Row 1 — directory */
    make("STATIC", "Directory:", SS_LEFT,            10, 14, 72, 20, 0);
    make("EDIT",   "",  WS_BORDER | ES_AUTOHSCROLL,  86, 10,565, 24, ID_EDIT_DIR);
    make("BUTTON", "Browse",  BS_PUSHBUTTON,         660, 10, 70, 24, ID_BTN_BROWSE);

    /* Row 2 — query + radio buttons */
    make("STATIC", "Search:", SS_LEFT,               10, 48, 72, 20, 0);
    make("EDIT",   "",  WS_BORDER | ES_AUTOHSCROLL,  86, 44,370, 24, ID_EDIT_QUERY);

    make("BUTTON", "By Name",
         BS_AUTORADIOBUTTON | WS_GROUP,              468, 46, 80, 20, ID_RADIO_NAME);
    make("BUTTON", "By Extension",
         BS_AUTORADIOBUTTON,                         554, 46,105, 20, ID_RADIO_EXT);
    make("BUTTON", "By Content",
         BS_AUTORADIOBUTTON,                         665, 46, 95, 20, ID_RADIO_CONTENT);

    CheckRadioButton(hwnd, ID_RADIO_NAME, ID_RADIO_CONTENT, ID_RADIO_NAME);

    /* Row 3 — action buttons + count */
    make("BUTTON", "Search", BS_DEFPUSHBUTTON,        10, 80, 90, 28, ID_BTN_SEARCH);
    make("BUTTON", "Clear",  BS_PUSHBUTTON,           110, 80, 90, 28, ID_BTN_CLEAR);
    make("STATIC", "Results: 0", SS_LEFT,             655, 86,140, 18, ID_STATIC_COUNT);

    /* ListView */
    listView_.create(hwnd, hInst_, 10, 118, 780, 410, ID_LISTVIEW);

    /* Status bar */
    make("STATIC", "Ready.", SS_LEFT,                  10,538,780, 18, ID_STATIC_STATUS);
}

/* ─────────────────────────────────────────────────────────────
   onCommand — dispatch button/menu events
──────────────────────────────────────────────────────────────── */
void MainWindow::onCommand(WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case ID_BTN_BROWSE: browseFolder(); break;
        case ID_BTN_SEARCH: runSearch();    break;
        case ID_BTN_CLEAR:  clearResults(); break;
    }
}

/* ─────────────────────────────────────────────────────────────
   browseFolder
──────────────────────────────────────────────────────────────── */
void MainWindow::browseFolder() {
    BROWSEINFOA bi{};
    bi.hwndOwner = hwnd_;
    bi.lpszTitle = "Select folder to search in:";
    bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (pidl) {
        char path[MAX_PATH];
        if (SHGetPathFromIDListA(pidl, path))
            SetDlgItemTextA(hwnd_, ID_EDIT_DIR, path);
        CoTaskMemFree(pidl);
    }
}

/* ─────────────────────────────────────────────────────────────
   buildStrategy — Simple Factory (DIP: returns interface ptr)
──────────────────────────────────────────────────────────────── */
ISearchStrategy* MainWindow::buildStrategy() const {
    if (IsDlgButtonChecked(hwnd_, ID_RADIO_EXT) == BST_CHECKED)
        return new ExtensionSearchStrategy();
    if (IsDlgButtonChecked(hwnd_, ID_RADIO_CONTENT) == BST_CHECKED)
        return new ContentSearchStrategy();
    return new FileNameSearchStrategy();   // default
}

/* ─────────────────────────────────────────────────────────────
   runSearch
──────────────────────────────────────────────────────────────── */
void MainWindow::runSearch() {
    std::string dir   = getDlgText(ID_EDIT_DIR);
    std::string query = getDlgText(ID_EDIT_QUERY);

    if (dir.empty()) {
        MessageBoxA(hwnd_, "Please select a directory first.",
                    "Missing Input", MB_ICONWARNING); return;
    }
    if (query.empty()) {
        MessageBoxA(hwnd_, "Please enter a search query.",
                    "Missing Input", MB_ICONWARNING); return;
    }

    setStatus("Searching...");
    SetCursor(LoadCursor(nullptr, IDC_WAIT));

    // DIP in action: we only know ISearchStrategy
    strategy_.reset(buildStrategy());
    results_ = strategy_->search(dir, query);

    SetCursor(LoadCursor(nullptr, IDC_ARROW));

    listView_.populate(results_);
    updateCount();

    std::ostringstream oss;
    oss << "Done — " << results_.size() << " file(s) found for \""
        << query << "\"  [" << strategy_->name() << "]";
    setStatus(oss.str());
}

/* ─────────────────────────────────────────────────────────────
   clearResults
──────────────────────────────────────────────────────────────── */
void MainWindow::clearResults() {
    listView_.clear();
    results_.clear();
    SetDlgItemTextA(hwnd_, ID_STATIC_COUNT, "Results: 0");
    setStatus("Cleared.");
}

/* ─────────────────────────────────────────────────────────────
   Helpers
──────────────────────────────────────────────────────────────── */
void MainWindow::setStatus(const std::string& text) {
    SetDlgItemTextA(hwnd_, ID_STATIC_STATUS, text.c_str());
}

void MainWindow::updateCount() {
    std::string s = "Results: " + std::to_string(results_.size());
    SetDlgItemTextA(hwnd_, ID_STATIC_COUNT, s.c_str());
}

std::string MainWindow::getDlgText(int ctrlId) const {
    char buf[MAX_PATH] = {};
    GetDlgItemTextA(hwnd_, ctrlId, buf, MAX_PATH);
    return buf;
}
