#include "../../include/controller/SearchController.h"
#include <sstream>

SearchController::SearchController(ISearchView& view)
    : view_(view) {}

void SearchController::runSearch() {
    const std::string dir = view_.directoryText();
    const std::string query = view_.queryText();

    if (dir.empty()) {
        view_.showWarning("Please select a directory first.");
        return;
    }

    if (query.empty()) {
        view_.showWarning("Please enter a search query.");
        return;
    }

    view_.setStatusText("Searching...");
    view_.setBusy(true);

    const auto& results = model_.search(dir, query, view_.selectedSearchMode());

    view_.setBusy(false);
    view_.showResults(results);
    view_.showResultCount(model_.resultCount());

    std::ostringstream oss;
    oss << "Done - " << model_.resultCount() << " file(s) found for \""
        << query << "\" [" << model_.activeStrategyName() << "]";
    view_.setStatusText(oss.str());
}

void SearchController::clearResults() {
    model_.clear();
    view_.showResults(model_.results());
    view_.showResultCount(0);
    view_.setStatusText("Cleared.");
}
