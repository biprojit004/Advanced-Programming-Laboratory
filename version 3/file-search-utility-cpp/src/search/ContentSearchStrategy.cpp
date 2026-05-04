#include "../../include/search/ContentSearchStrategy.h"
#include "../../include/utils/FileUtils.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

/* ─────────────────────────────────────────────────────────────
   isTextFile — only scan known text formats
──────────────────────────────────────────────────────────────── */
bool ContentSearchStrategy::isTextFile(const std::string& fileName) {
    static const char* TEXT_EXTS[] = {
        ".txt", ".cpp", ".c", ".h", ".hpp", ".java",
        ".py",  ".js",  ".ts", ".html", ".css",
        ".xml", ".json", ".csv", ".md", ".ini",
        ".bat", ".sh",  ".log", ".properties", nullptr
    };
    std::string ext = getExtension(fileName);
    for (int i = 0; TEXT_EXTS[i]; ++i)
        if (FileUtils::equalsIgnoreCase(ext, TEXT_EXTS[i])) return true;
    return false;
}

/* ─────────────────────────────────────────────────────────────
   fileContains — line-by-line scan (memory efficient)
──────────────────────────────────────────────────────────────── */
bool ContentSearchStrategy::fileContains(const std::string& path,
                                         const std::string& query)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (FileUtils::containsIgnoreCase(line, query)) return true;
    }
    return false;
}

/* ─────────────────────────────────────────────────────────────
   getExtension
──────────────────────────────────────────────────────────────── */
std::string ContentSearchStrategy::getExtension(const std::string& fileName) {
    auto pos = fileName.rfind('.');
    if (pos == std::string::npos || pos == 0) return "";
    return fileName.substr(pos);
}

/* ─────────────────────────────────────────────────────────────
   traverse
──────────────────────────────────────────────────────────────── */
void ContentSearchStrategy::traverse(const std::string& dir,
                                     const std::string& query,
                                     std::vector<SearchResult>& results) const
{
    if (results.size() >= 2048) return;

    std::string pattern = dir + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(pattern.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::string name = fd.cFileName;
        if (name == "." || name == "..") continue;

        std::string fullPath = dir + "\\" + name;

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            traverse(fullPath, query, results);
        } else if (isTextFile(name) && fileContains(fullPath, query)) {
            SearchResult r;
            r.fullPath      = fullPath;
            r.fileName      = name;
            r.extension     = getExtension(name);
            r.sizeBytes     = FileUtils::extractSize(fd);
            r.sizeFormatted = FileUtils::formatSize(r.sizeBytes);
            results.push_back(std::move(r));
        }
    } while (FindNextFileA(hFind, &fd) && results.size() < 2048);

    FindClose(hFind);
}

/* ─────────────────────────────────────────────────────────────
   Public API
──────────────────────────────────────────────────────────────── */
std::vector<SearchResult> ContentSearchStrategy::search(
    const std::string& rootDir,
    const std::string& query) const
{
    std::vector<SearchResult> results;
    traverse(rootDir, query, results);
    return results;
}

std::string ContentSearchStrategy::name() const {
    return "By Content";
}
