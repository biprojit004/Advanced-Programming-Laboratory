package com.filesearch.model;

import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.io.IOException;
import java.nio.file.Files;
import java.time.Instant;

// Single Responsibility: শুধু একটা search result এর data hold করে
public class SearchResult {
    private final Path path;
    private final boolean isDirectory;
    private final long size;
    private final Instant lastModified;

    public SearchResult(Path path) {
        this.path = path;
        this.isDirectory = Files.isDirectory(path);
        long tempSize = 0;
        Instant tempTime = Instant.now();
        try {
            BasicFileAttributes attrs = Files.readAttributes(path, BasicFileAttributes.class);
            tempSize = attrs.size();
            tempTime = attrs.lastModifiedTime().toInstant();
        } catch (IOException ignored) {}
        this.size = tempSize;
        this.lastModified = tempTime;
    }

    public Path getPath() { return path; }
    public String getName() { return path.getFileName().toString(); }
    public boolean isDirectory() { return isDirectory; }
    public long getSize() { return size; }
    public Instant getLastModified() { return lastModified; }

    public String getFormattedSize() {
        if (isDirectory) return "<DIR>";
        if (size < 1024) return size + " B";
        if (size < 1024 * 1024) return String.format("%.1f KB", size / 1024.0);
        return String.format("%.1f MB", size / (1024.0 * 1024));
    }

    @Override
    public String toString() {
        return String.format("%-40s %-8s %s",
            getName(),
            getFormattedSize(),
            path.toAbsolutePath()
        );
    }
}
