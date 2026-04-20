#pragma once

#include "../model/SearchResult.h"
#include <vector>
#include <string>

/*
 * ISearchStrategy.h — Interface (OCP + DIP)
 *
 * Open/Closed Principle:
 *   New search strategies (e.g. RegexSearch, SizeSearch) can be added
 *   by implementing this interface — no existing code needs to change.
 *
 * Dependency Inversion Principle:
 *   High-level classes (MainWindow, SearchService) depend on THIS
 *   abstraction, never on concrete strategy classes directly.
 */
class ISearchStrategy {
public:
    virtual ~ISearchStrategy() = default;

    // Execute the search and return matching results
    virtual std::vector<SearchResult> search(
        const std::string& rootDir,
        const std::string& query
    ) const = 0;

    // Human-readable label shown in the UI
    virtual std::string name() const = 0;
};
