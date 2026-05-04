#include "../../include/model/SearchModel.h"
#include "../../include/search/ContentSearchStrategy.h"
#include "../../include/search/ExtensionSearchStrategy.h"
#include "../../include/search/FileNameSearchStrategy.h"
#include "../../include/search/ISearchStrategy.h"
#include <memory>

namespace {
std::unique_ptr<ISearchStrategy> makeStrategy(SearchMode mode) {
    switch (mode) {
        case SearchMode::Extension:
            return std::make_unique<ExtensionSearchStrategy>();
        case SearchMode::Content:
            return std::make_unique<ContentSearchStrategy>();
        case SearchMode::Name:
        default:
            return std::make_unique<FileNameSearchStrategy>();
    }
}
}

const std::vector<SearchResult>& SearchModel::search(const std::string& rootDir,
                                                     const std::string& query,
                                                     SearchMode mode) {
    auto strategy = makeStrategy(mode);
    activeStrategyName_ = strategy->name();
    results_ = strategy->search(rootDir, query);
    return results_;
}

void SearchModel::clear() {
    results_.clear();
    activeStrategyName_.clear();
}
