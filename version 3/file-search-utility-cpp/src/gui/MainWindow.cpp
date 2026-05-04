#include "../../include/gui/MainWindow.h"
#include <commctrl.h>
#include <shlobj.h>

MainWindow::MainWindow()
    : controller_(std::make_unique<SearchController>(*this)) {}

bool MainWindow::create(HINSTANCE hInstance, int nCmdShow) {
    hInst_ = hInstance;

    INITCOMMONCONTROLSEX icex{ sizeof(icex), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icex);

    WNDCLASSA wc{};
    wc.lpfnWndProc = MainWindow::wndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "FileSearchCpp";
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    if (!RegisterClassA(&wc)) return false;

    hwnd_ = CreateWindowExA(
        0, "FileSearchCpp",
        "File Search Utility [C++ / MVC]",
        (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance,
        this
    );

    if (!hwnd_) return false;
    ShowWindow(hwnd_, nCmdShow);
    UpdateWindow(hwnd_);
    return true;
}

LRESULT CALLBACK MainWindow::wndProc(HWND hwnd, UINT msg,
                                      WPARAM wParam, LPARAM lParam) {
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

LRESULT MainWindow::handleMessage(HWND hwnd, UINT msg,
                                   WPARAM wParam, LPARAM lParam) {
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

void MainWindow::onCreate(HWND hwnd) {
    hwnd_ = hwnd;

    auto make = [&](const char* cls, const char* txt,
                    DWORD style, int x, int y, int w, int h, int id) {
        return CreateWindowA(cls, txt, WS_CHILD | WS_VISIBLE | style,
                             x, y, w, h, hwnd,
                             reinterpret_cast<HMENU>((LONG_PTR)id),
                             hInst_, nullptr);
    };

    make("STATIC", "Directory:", SS_LEFT, 10, 14, 72, 20, 0);
    make("EDIT", "", WS_BORDER | ES_AUTOHSCROLL, 86, 10, 565, 24, ID_EDIT_DIR);
    make("BUTTON", "Browse", BS_PUSHBUTTON, 660, 10, 70, 24, ID_BTN_BROWSE);

    make("STATIC", "Search:", SS_LEFT, 10, 48, 72, 20, 0);
    make("EDIT", "", WS_BORDER | ES_AUTOHSCROLL, 86, 44, 370, 24, ID_EDIT_QUERY);

    make("BUTTON", "By Name",
         BS_AUTORADIOBUTTON | WS_GROUP, 468, 46, 80, 20, ID_RADIO_NAME);
    make("BUTTON", "By Extension",
         BS_AUTORADIOBUTTON, 554, 46, 105, 20, ID_RADIO_EXT);
    make("BUTTON", "By Content",
         BS_AUTORADIOBUTTON, 665, 46, 95, 20, ID_RADIO_CONTENT);

    CheckRadioButton(hwnd, ID_RADIO_NAME, ID_RADIO_CONTENT, ID_RADIO_NAME);

    make("BUTTON", "Search", BS_DEFPUSHBUTTON, 10, 80, 90, 28, ID_BTN_SEARCH);
    make("BUTTON", "Clear", BS_PUSHBUTTON, 110, 80, 90, 28, ID_BTN_CLEAR);
    make("STATIC", "Results: 0", SS_LEFT, 655, 86, 140, 18, ID_STATIC_COUNT);

    listView_.create(hwnd, hInst_, 10, 118, 780, 410, ID_LISTVIEW);

    make("STATIC", "Ready.", SS_LEFT, 10, 538, 780, 18, ID_STATIC_STATUS);
}

void MainWindow::onCommand(WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case ID_BTN_BROWSE:
            browseFolder();
            break;
        case ID_BTN_SEARCH:
            controller_->runSearch();
            break;
        case ID_BTN_CLEAR:
            controller_->clearResults();
            break;
    }
}

void MainWindow::browseFolder() {
    BROWSEINFOA bi{};
    bi.hwndOwner = hwnd_;
    bi.lpszTitle = "Select folder to search in:";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);
    if (pidl) {
        char path[MAX_PATH];
        if (SHGetPathFromIDListA(pidl, path))
            SetDlgItemTextA(hwnd_, ID_EDIT_DIR, path);
        CoTaskMemFree(pidl);
    }
}

std::string MainWindow::directoryText() const {
    return getDlgText(ID_EDIT_DIR);
}

std::string MainWindow::queryText() const {
    return getDlgText(ID_EDIT_QUERY);
}

SearchMode MainWindow::selectedSearchMode() const {
    if (IsDlgButtonChecked(hwnd_, ID_RADIO_EXT) == BST_CHECKED)
        return SearchMode::Extension;
    if (IsDlgButtonChecked(hwnd_, ID_RADIO_CONTENT) == BST_CHECKED)
        return SearchMode::Content;
    return SearchMode::Name;
}

void MainWindow::showWarning(const std::string& message) {
    MessageBoxA(hwnd_, message.c_str(), "Missing Input", MB_ICONWARNING);
}

void MainWindow::setBusy(bool busy) {
    SetCursor(LoadCursor(nullptr, busy ? IDC_WAIT : IDC_ARROW));
}

void MainWindow::showResults(const std::vector<SearchResult>& results) {
    listView_.populate(results);
}

void MainWindow::showResultCount(std::size_t count) {
    const std::string text = "Results: " + std::to_string(count);
    SetDlgItemTextA(hwnd_, ID_STATIC_COUNT, text.c_str());
}

void MainWindow::setStatusText(const std::string& text) {
    SetDlgItemTextA(hwnd_, ID_STATIC_STATUS, text.c_str());
}

std::string MainWindow::getDlgText(int ctrlId) const {
    char buf[MAX_PATH] = {};
    GetDlgItemTextA(hwnd_, ctrlId, buf, MAX_PATH);
    return buf;
}
