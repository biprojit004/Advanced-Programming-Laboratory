#pragma once

#include "SearchResult.h"
#include <cstddef>
#include <string>
#include <vector>

enum class SearchMode {
    Name,
    Extension,
    Content
};

class SearchModel {
public:
    const std::vector<SearchResult>& search(const std::string& rootDir,
                                            const std::string& query,
                                            SearchMode mode);

    void clear();

    const std::vector<SearchResult>& results() const { return results_; }
    std::size_t resultCount() const { return results_.size(); }
    const std::string& activeStrategyName() const { return activeStrategyName_; }

private:
    std::vector<SearchResult> results_;
    std::string activeStrategyName_;
};
