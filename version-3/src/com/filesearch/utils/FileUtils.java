package com.filesearch.utils;

import java.nio.file.Files;
import java.nio.file.Paths;

// Single Responsibility: শুধু utility/helper methods
public class FileUtils {

    private FileUtils() {} // Utility class — instantiate করা যাবে না

    public static boolean isValidDirectory(String path) {
        if (path == null || path.trim().isEmpty()) return false;
        return Files.isDirectory(Paths.get(path));
    }

    public static boolean isValidQuery(String query) {
        return query != null && !query.trim().isEmpty();
    }

    public static String getDefaultSearchPath() {
        // Windows এ C:\ , Linux/Mac এ home directory
        String os = System.getProperty("os.name").toLowerCase();
        if (os.contains("win")) return "C:\\";
        return System.getProperty("user.home");
    }

    public static String formatResultCount(int count) {
        if (count == 0) return "কোনো result পাওয়া যায়নি।";
        return count + " টি file/folder পাওয়া গেছে।";
    }
}
