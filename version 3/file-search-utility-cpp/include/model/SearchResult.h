#pragma once

#include <string>
#include <windows.h>

/*
 * SearchResult.h — Model (SRP)
 *
 * Single Responsibility: holds data for ONE matched file.
 * No search logic, no display logic — pure data container.
 */
struct SearchResult {
    std::string fullPath;       // e.g. C:\Users\...\notes.txt
    std::string fileName;       // e.g. notes.txt
    std::string extension;      // e.g. .txt
    std::string sizeFormatted;  // e.g. 12 KB
    LONGLONG    sizeBytes = 0;
};
