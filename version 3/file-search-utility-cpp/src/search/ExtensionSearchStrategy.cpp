#include "../../include/search/ExtensionSearchStrategy.h"
#include "../../include/utils/FileUtils.h"
#include <windows.h>
#include <algorithm>
#include <cctype>

/* ─────────────────────────────────────────────────────────────
   normalizeExtension — "txt" or ".txt" → ".txt" (lowercase)
──────────────────────────────────────────────────────────────── */
std::string ExtensionSearchStrategy::normalizeExtension(const std::string& query) {
    std::string ext = query;
    // Lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    // Ensure leading dot
    if (!ext.empty() && ext[0] != '.') ext = "." + ext;
    return ext;
}

/* ─────────────────────────────────────────────────────────────
   getExtension
──────────────────────────────────────────────────────────────── */
std::string ExtensionSearchStrategy::getExtension(const std::string& fileName) {
    auto pos = fileName.rfind('.');
    if (pos == std::string::npos || pos == 0) return "";
    std::string ext = fileName.substr(pos);
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return ext;
}

/* ─────────────────────────────────────────────────────────────
   traverse
──────────────────────────────────────────────────────────────── */
void ExtensionSearchStrategy::traverse(const std::string& dir,
                                       const std::string& normalizedExt,
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
            traverse(fullPath, normalizedExt, results);
        } else if (getExtension(name) == normalizedExt) {
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
std::vector<SearchResult> ExtensionSearchStrategy::search(
    const std::string& rootDir,
    const std::string& query) const
{
    std::vector<SearchResult> results;
    traverse(rootDir, normalizeExtension(query), results);
    return results;
}

std::string ExtensionSearchStrategy::name() const {
    return "By Extension";
}
