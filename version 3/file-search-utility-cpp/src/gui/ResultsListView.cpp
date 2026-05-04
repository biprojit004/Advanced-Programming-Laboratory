#include "../../include/gui/ResultsListView.h"
#include <string>

/* ─────────────────────────────────────────────────────────────
   create
──────────────────────────────────────────────────────────────── */
bool ResultsListView::create(HWND parent, HINSTANCE hInstance,
                              int x, int y, int width, int height,
                              int controlId)
{
    hList_ = CreateWindowExA(
        WS_EX_CLIENTEDGE, WC_LISTVIEWA, "",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        x, y, width, height,
        parent, reinterpret_cast<HMENU>((LONG_PTR)controlId),
        hInstance, nullptr
    );

    if (!hList_) return false;

    ListView_SetExtendedListViewStyle(hList_,
        LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    addColumns();
    return true;
}

/* ─────────────────────────────────────────────────────────────
   addColumns — define the 4 result columns
──────────────────────────────────────────────────────────────── */
void ResultsListView::addColumns() {
    struct ColDef { const char* label; int width; };
    static const ColDef cols[] = {
        { "File Name",  220 },
        { "Size",        65 },
        { "Extension",   70 },
        { "Full Path",  425 },
    };

    LVCOLUMNA col{};
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    for (int i = 0; i < 4; ++i) {
        col.iSubItem = i;
        col.cx       = cols[i].width;
        col.pszText  = const_cast<char*>(cols[i].label);
        ListView_InsertColumn(hList_, i, &col);
    }
}

/* ─────────────────────────────────────────────────────────────
   populate — fill ListView rows from result vector
──────────────────────────────────────────────────────────────── */
void ResultsListView::populate(const std::vector<SearchResult>& results) {
    ListView_DeleteAllItems(hList_);

    for (int i = 0; i < static_cast<int>(results.size()); ++i) {
        const SearchResult& r = results[i];

        LVITEMA item{};
        item.mask     = LVIF_TEXT;
        item.iItem    = i;
        item.iSubItem = 0;
        item.pszText  = const_cast<char*>(r.fileName.c_str());
        ListView_InsertItem(hList_, &item);

        auto setSubItem = [&](int subItem, const char* text) {
            LVITEMA sub{};
            sub.iSubItem = subItem;
            sub.pszText = const_cast<char*>(text);
            SendMessageA(hList_, LVM_SETITEMTEXTA, i,
                         reinterpret_cast<LPARAM>(&sub));
        };

        setSubItem(1, r.sizeFormatted.c_str());
        setSubItem(2, r.extension.empty() ? "(none)" : r.extension.c_str());
        setSubItem(3, r.fullPath.c_str());
    }
}

/* ─────────────────────────────────────────────────────────────
   clear
──────────────────────────────────────────────────────────────── */
void ResultsListView::clear() {
    if (hList_) ListView_DeleteAllItems(hList_);
}
