package com.filesearch.main;

import com.filesearch.gui.ConsoleUI;
import com.filesearch.search.FileSearcher;
import com.filesearch.search.RecursiveFileSearcher;

// Dependency Inversion: Main এ dependency wire করা হয়
// এখানে শুধু object তৈরি করে UI তে inject করা হয়
public class Main {
    public static void main(String[] args) {
        FileSearcher searcher = new RecursiveFileSearcher();
        ConsoleUI ui = new ConsoleUI(searcher);
        ui.start();
    }
}
