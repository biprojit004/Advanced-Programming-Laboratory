#pragma once

#include "../model/SearchResult.h"
#include <windows.h>
#include <commctrl.h>
#include <vector>

/*
 * ResultsListView.h — Sub-component (SRP)
 *
 * Single Responsibility: owns and manages the Win32 ListView
 * control that shows search results. Knows nothing about search
 * logic or window layout.
 */
class ResultsListView {
public:
    // Create the ListView inside the given parent window
    bool create(HWND parent, HINSTANCE hInstance, int x, int y,
                int width, int height, int controlId);

    // Replace all rows with the new result set
    void populate(const std::vector<SearchResult>& results);

    // Remove every row
    void clear();

    HWND handle() const { return hList_; }

private:
    HWND hList_ = nullptr;

    void addColumns();
};
