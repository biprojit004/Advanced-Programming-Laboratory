#include "../../include/search/FileNameSearchStrategy.h"
#include "../../include/utils/FileUtils.h"
#include <windows.h>

/* ─────────────────────────────────────────────────────────────
   getExtension — ".txt" from "notes.txt", "" if none
──────────────────────────────────────────────────────────────── */
std::string FileNameSearchStrategy::getExtension(const std::string& fileName) {
    auto pos = fileName.rfind('.');
    if (pos == std::string::npos || pos == 0) return "";
    return fileName.substr(pos);
}

/* ─────────────────────────────────────────────────────────────
   traverse — recursive Win32 directory walk
──────────────────────────────────────────────────────────────── */
void FileNameSearchStrategy::traverse(const std::string& dir,
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
        } else if (FileUtils::containsIgnoreCase(name, query)) {
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
std::vector<SearchResult> FileNameSearchStrategy::search(
    const std::string& rootDir,
    const std::string& query) const
{
    std::vector<SearchResult> results;
    traverse(rootDir, query, results);
    return results;
}

std::string FileNameSearchStrategy::name() const {
    return "By Name";
}
