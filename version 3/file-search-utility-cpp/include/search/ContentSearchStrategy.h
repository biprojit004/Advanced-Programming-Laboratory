#pragma once

#include "ISearchStrategy.h"

/*
 * ContentSearchStrategy.h — Concrete Strategy (SRP + LSP)
 *
 * Single Responsibility: ONLY searches inside file contents.
 * Opens text files line-by-line — never loads the whole file.
 */
class ContentSearchStrategy : public ISearchStrategy {
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

    static bool isTextFile(const std::string& fileName);
    static bool fileContains(const std::string& path, const std::string& query);
    static std::string getExtension(const std::string& fileName);
};
