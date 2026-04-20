#pragma once

#include "ISearchStrategy.h"

/*
 * ExtensionSearchStrategy.h — Concrete Strategy (SRP + LSP)
 *
 * Single Responsibility: ONLY matches files by their extension.
 */
class ExtensionSearchStrategy : public ISearchStrategy {
public:
    std::vector<SearchResult> search(
        const std::string& rootDir,
        const std::string& query
    ) const override;

    std::string name() const override;

private:
    void traverse(
        const std::string& dir,
        const std::string& normalizedExt,
        std::vector<SearchResult>& results
    ) const;

    static std::string normalizeExtension(const std::string& query);
    static std::string getExtension(const std::string& fileName);
};
