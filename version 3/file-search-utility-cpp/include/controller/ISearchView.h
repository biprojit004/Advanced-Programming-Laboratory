#pragma once

#include "../model/SearchModel.h"
#include <cstddef>
#include <string>
#include <vector>

class ISearchView {
public:
    virtual ~ISearchView() = default;

    virtual std::string directoryText() const = 0;
    virtual std::string queryText() const = 0;
    virtual SearchMode selectedSearchMode() const = 0;

    virtual void showWarning(const std::string& message) = 0;
    virtual void setBusy(bool busy) = 0;
    virtual void showResults(const std::vector<SearchResult>& results) = 0;
    virtual void showResultCount(std::size_t count) = 0;
    virtual void setStatusText(const std::string& text) = 0;
};
