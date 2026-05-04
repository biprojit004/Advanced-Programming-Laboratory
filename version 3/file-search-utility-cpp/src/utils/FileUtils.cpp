#include "../../include/utils/FileUtils.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

namespace FileUtils {

/* ─────────────────────────────────────────────────────────────
   containsIgnoreCase
──────────────────────────────────────────────────────────────── */
bool containsIgnoreCase(const std::string& haystack,
                        const std::string& needle)
{
    if (needle.empty()) return true;

    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(),   needle.end(),
        [](char a, char b) {
            return std::tolower((unsigned char)a) ==
                   std::tolower((unsigned char)b);
        }
    );
    return it != haystack.end();
}

/* ─────────────────────────────────────────────────────────────
   equalsIgnoreCase
──────────────────────────────────────────────────────────────── */
bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    return std::equal(a.begin(), a.end(), b.begin(),
        [](char x, char y) {
            return std::tolower((unsigned char)x) ==
                   std::tolower((unsigned char)y);
        }
    );
}

/* ─────────────────────────────────────────────────────────────
   formatSize — bytes → "1.2 KB" / "3.4 MB" / "2.10 GB"
──────────────────────────────────────────────────────────────── */
std::string formatSize(LONGLONG bytes) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);

    if (bytes < 0)
        return "Unknown";
    else if (bytes < 1024LL)
        oss << bytes << " B";
    else if (bytes < 1024LL * 1024)
        oss << (bytes / 1024.0) << " KB";
    else if (bytes < 1024LL * 1024 * 1024)
        oss << (bytes / (1024.0 * 1024)) << " MB";
    else
        oss << std::setprecision(2)
            << (bytes / (1024.0 * 1024 * 1024)) << " GB";

    return oss.str();
}

/* ─────────────────────────────────────────────────────────────
   extractSize — pull LONGLONG from WIN32_FIND_DATAA
──────────────────────────────────────────────────────────────── */
LONGLONG extractSize(const WIN32_FIND_DATAA& fd) {
    LARGE_INTEGER li;
    li.LowPart  = fd.nFileSizeLow;
    li.HighPart = static_cast<LONG>(fd.nFileSizeHigh);
    return li.QuadPart;
}

} // namespace FileUtils
