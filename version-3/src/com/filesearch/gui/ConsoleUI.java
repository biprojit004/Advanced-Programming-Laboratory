package com.filesearch.gui;

import com.filesearch.model.SearchResult;
import com.filesearch.search.FileSearcher;
import com.filesearch.utils.FileUtils;

import java.util.List;
import java.util.Scanner;

// Single Responsibility: শুধু user interaction handle করে
// Dependency Inversion: FileSearcher interface এর উপর depend করে, concrete class এ না
public class ConsoleUI {

    private final FileSearcher searcher;
    private final Scanner scanner;

    public ConsoleUI(FileSearcher searcher) {
        this.searcher = searcher;
        this.scanner = new Scanner(System.in);
    }

    public void start() {
        printBanner();

        while (true) {
            System.out.println("\n==============================");
            System.out.println("  1. নতুন Search করো");
            System.out.println("  2. Exit");
            System.out.println("==============================");
            System.out.print("Choice: ");

            String choice = scanner.nextLine().trim();

            switch (choice) {
                case "1" -> performSearch();
                case "2" -> {
                    System.out.println("\nধন্যবাদ! Program বন্ধ হচ্ছে...");
                    return;
                }
                default -> System.out.println("[!] Valid option দাও (1 বা 2)");
            }
        }
    }

    private void performSearch() {
        // Root path input
        System.out.println("\nSearch শুরু করার folder path দাও");
        System.out.println("(Enter চাপলে default: " + FileUtils.getDefaultSearchPath() + ")");
        System.out.print("Path: ");
        String rootPath = scanner.nextLine().trim();

        if (rootPath.isEmpty()) rootPath = FileUtils.getDefaultSearchPath();

        if (!FileUtils.isValidDirectory(rootPath)) {
            System.out.println("[ERROR] এই path টা valid directory না!");
            return;
        }

        // Query input
        System.out.print("File/Folder এর নাম লেখো (যেটা খুঁজবে): ");
        String query = scanner.nextLine().trim();

        if (!FileUtils.isValidQuery(query)) {
            System.out.println("[ERROR] Search query খালি রাখা যাবে না!");
            return;
        }

        // Search
        System.out.println("\nSearching... অপেক্ষা করো।\n");
        long startTime = System.currentTimeMillis();
        List<SearchResult> results = searcher.search(rootPath, query);
        long elapsed = System.currentTimeMillis() - startTime;

        // Show results
        printResults(results, query, elapsed);
    }

    private void printResults(List<SearchResult> results, String query, long elapsed) {
        System.out.println("┌─────────────────────────────────────────────────────────────────────┐");
        System.out.printf("│  Search: %-60s│%n", "\"" + query + "\"");
        System.out.println("├──────────────────────┬──────────┬──────────────────────────────────┤");
        System.out.printf("│ %-20s │ %-8s │ %-32s │%n", "নাম", "Size", "Full Path");
        System.out.println("├──────────────────────┼──────────┼──────────────────────────────────┤");

        if (results.isEmpty()) {
            System.out.printf("│ %-63s │%n", "  কোনো result পাওয়া যায়নি।");
        } else {
            for (SearchResult r : results) {
                String name = truncate(r.getName(), 20);
                String size = truncate(r.getFormattedSize(), 8);
                String path = truncate(r.getPath().toAbsolutePath().toString(), 32);
                System.out.printf("│ %-20s │ %-8s │ %-32s │%n", name, size, path);
            }
        }

        System.out.println("└──────────────────────┴──────────┴──────────────────────────────────┘");
        System.out.printf("  %s  (%.2f seconds)%n",
            FileUtils.formatResultCount(results.size()), elapsed / 1000.0);
    }

    private void printBanner() {
        System.out.println("╔══════════════════════════════════════╗");
        System.out.println("║       FILE SEARCH UTILITY v2.0       ║");
        System.out.println("║         Java Edition (SOLID)         ║");
        System.out.println("╚══════════════════════════════════════╝");
    }

    private String truncate(String s, int max) {
        if (s.length() <= max) return s;
        return s.substring(0, max - 2) + "..";
    }
}
