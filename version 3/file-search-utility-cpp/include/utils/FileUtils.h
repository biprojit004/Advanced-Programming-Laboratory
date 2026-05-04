#pragma once

#include <string>
#include <windows.h>

/*
 * FileUtils.h  (ISP)
 *
 * Interface Segregation Principle:
 *   Small, focused utility functions — no class forces callers to
 *   depend on helpers they don't use.
 */
namespace FileUtils {

    // Case-insensitive substring search (like strstr, ignores case)
    bool containsIgnoreCase(const std::string& haystack,
                            const std::string& needle);

    // Case-insensitive equality check
    bool equalsIgnoreCase(const std::string& a, const std::string& b);

    // Format a raw byte count → "1.2 KB" / "3.4 MB" / "2.10 GB"
    std::string formatSize(LONGLONG bytes);

    // Build a SearchResult's sizeBytes + sizeFormatted from WIN32_FIND_DATAA
    LONGLONG extractSize(const WIN32_FIND_DATAA& fd);

}
