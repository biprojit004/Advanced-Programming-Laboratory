#pragma once

#include "ISearchStrategy.h"

/*
 * FileNameSearchStrategy.h — Concrete Strategy (SRP + LSP)
 *
 * Single Responsibility: ONLY handles name-based matching.
 * Liskov Substitution: Can replace any ISearchStrategy without
 *   breaking the caller.
 */
class FileNameSearchStrategy : public ISearchStrategy {
public:
    std::vector<SearchResult> search(
        const std::string& rootDir,
        const std::string& query
    ) const override;

    std::string name() const override;

private:
    void traverse(
        const std::string& dir,
        const std::string& query,
        std::vector<SearchResult>& results
    ) const;

    static std::string getExtension(const std::string& fileName);
};
