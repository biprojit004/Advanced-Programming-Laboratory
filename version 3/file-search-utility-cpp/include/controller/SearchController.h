#pragma once

#include "ISearchView.h"
#include "../model/SearchModel.h"

class SearchController {
public:
    explicit SearchController(ISearchView& view);

    void runSearch();
    void clearResults();

private:
    ISearchView& view_;
    SearchModel model_;
};
