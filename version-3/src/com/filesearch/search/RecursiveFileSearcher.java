package com.filesearch.search;

import com.filesearch.model.SearchResult;
import java.io.IOException;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.List;

// Open/Closed Principle: নতুন search logic আসলে এই class extend করা যাবে
// Single Responsibility: শুধু recursive file search করে
public class RecursiveFileSearcher implements FileSearcher {

    @Override
    public List<SearchResult> search(String rootPath, String query) {
        List<SearchResult> results = new ArrayList<>();
        Path root = Paths.get(rootPath);

        if (!Files.exists(root)) {
            System.out.println("[ERROR] Path exists না: " + rootPath);
            return results;
        }

        String lowerQuery = query.toLowerCase();

        try {
            Files.walkFileTree(root, new SimpleFileVisitor<Path>() {

                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) {
                    if (file.getFileName().toString().toLowerCase().contains(lowerQuery)) {
                        results.add(new SearchResult(file));
                    }
                    return FileVisitResult.CONTINUE;
                }

                @Override
                public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) {
                    if (!dir.equals(root)) {
                        if (dir.getFileName().toString().toLowerCase().contains(lowerQuery)) {
                            results.add(new SearchResult(dir));
                        }
                    }
                    return FileVisitResult.CONTINUE;
                }

                @Override
                public FileVisitResult visitFileFailed(Path file, IOException exc) {
                    // Access denied হলে skip করো, crash করো না
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch (IOException e) {
            System.out.println("[ERROR] Search failed: " + e.getMessage());
        }

        return results;
    }
}
