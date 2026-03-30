# File Search Utility — Java Edition(SOLID PRINCIPALS)

![Java](https://img.shields.io/badge/Java-11%2B-orange?style=flat-square&logo=java)
![SOLID](https://img.shields.io/badge/Design-SOLID%20Principles-blue?style=flat-square)
![Version](https://img.shields.io/badge/Version-3.0-green?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey?style=flat-square)

A recursive file and folder search utility built in **Java**. This project demonstrates the practical application of **SOLID design principles** through a clean, layered package structure — rewritten from the original C implementation for better maintainability and portability.

---

## Table of Contents

- [Project Structure](#project-structure)
- [SOLID Principles](#solid-principles)
- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Version History](#version-history)

---

## Project Structure

The project follows a standard Java package layout where each package has a single, well-defined responsibility. This separation makes the codebase easy to navigate, maintain, and extend.

```
FileSearchUtility/
│
├── src/
│   └── com/
│       └── filesearch/
│           │
│           ├── main/
│           │   └── Main.java                   # Entry point — wires all dependencies together
│           │
│           ├── model/
│           │   └── SearchResult.java           # Data model — holds file/folder metadata
│           │
│           ├── search/
│           │   ├── FileSearcher.java           # Interface — defines the search contract
│           │   └── RecursiveFileSearcher.java  # Implementation — recursive traversal logic
│           │
│           ├── gui/
│           │   └── ConsoleUI.java              # UI layer — handles all input and output
│           │
│           └── utils/
│               └── FileUtils.java              # Utilities — validation and formatting helpers
│
├── build.bat                                   # Compiles the project (Windows)
├── run.bat                                     # Runs the compiled project (Windows)
└── .gitignore
```

### Why This Structure?

| Package | Role | Reason for Separation |
|---|---|---|
| `main` | Entry point | Keeps dependency wiring isolated from business logic |
| `model` | Data representation | Changes to file metadata do not affect search or UI logic |
| `search` | Core logic | Search algorithm can be swapped without touching the UI |
| `gui` | User interaction | UI changes do not affect any underlying logic |
| `utils` | Shared helpers | Reusable across any layer without circular dependency |

---

## SOLID Principles

Every design decision in this project maps directly to one or more of the five SOLID principles. The following explains how each principle is applied and which classes are involved.

---

### S — Single Responsibility Principle
> *A class should have only one reason to change.*

Each class in this project is responsible for exactly one thing. If requirements change, only the relevant class needs to be modified — no other class is affected.

| Class | Single Responsibility |
|---|---|
| `SearchResult` | Stores and formats metadata (name, size, path) for one search result |
| `RecursiveFileSearcher` | Traverses the file system recursively and matches file names |
| `ConsoleUI` | Displays the menu, collects user input, and prints results |
| `FileUtils` | Validates paths and queries, formats output strings |
| `Main` | Constructs objects and injects dependencies — nothing else |

**Example:** If the display format of results needs to change, only `ConsoleUI` is modified. If the search algorithm changes, only `RecursiveFileSearcher` is modified. No other class is touched.

---

### O — Open/Closed Principle
> *A class should be open for extension but closed for modification.*

The `FileSearcher` interface acts as the extension point. New search behaviors can be added by implementing this interface — without modifying any existing class.

```
FileSearcher (interface)
│
├── RecursiveFileSearcher   ← current implementation
├── ExtensionFilterSearcher ← can be added later (e.g., search only .pdf files)
└── RegexFileSearcher       ← can be added later (e.g., pattern-based search)
```

Adding a new search type requires **zero changes** to `ConsoleUI`, `Main`, or any other class.

---

### L — Liskov Substitution Principle
> *Subtypes must be substitutable for their base type without altering program correctness.*

`ConsoleUI` accepts any object of type `FileSearcher`. Whether it receives a `RecursiveFileSearcher` or any future implementation, the behavior of the UI layer remains correct and consistent.

```java
// ConsoleUI works correctly with any FileSearcher implementation
FileSearcher searcher = new RecursiveFileSearcher();
ConsoleUI ui = new ConsoleUI(searcher);
```

Swapping `RecursiveFileSearcher` with another implementation does not require any changes to `ConsoleUI`.

---

### I — Interface Segregation Principle
> *A class should not be forced to implement methods it does not use.*

The `FileSearcher` interface is intentionally kept small and focused. It defines only one method — `search()` — which is the only operation any search implementation needs to fulfill.

```java
public interface FileSearcher {
    List<SearchResult> search(String rootPath, String query);
}
```

No implementation is burdened with unrelated methods. Each class implements only what it actually needs.

---

### D — Dependency Inversion Principle
> *High-level modules should not depend on low-level modules. Both should depend on abstractions.*

`ConsoleUI` (high-level) does not import or reference `RecursiveFileSearcher` (low-level) at any point. Both depend on the `FileSearcher` interface (abstraction). The dependency is injected from `Main` through the constructor.

```java
// Main.java — only place where the concrete class appears
FileSearcher searcher = new RecursiveFileSearcher();
ConsoleUI ui = new ConsoleUI(searcher); // UI depends on interface, not implementation
ui.start();
```

```java
// ConsoleUI.java — depends only on the interface
public class ConsoleUI {
    private final FileSearcher searcher; // interface type, not RecursiveFileSearcher

    public ConsoleUI(FileSearcher searcher) {
        this.searcher = searcher;
    }
}
```

This means `ConsoleUI` can be tested independently by passing a mock `FileSearcher` — no real file system access required.

---

## Overview

The **File Search Utility** allows users to search for files and folders across any directory by entering a partial or full name. The program recursively traverses all subdirectories and displays matching results with their name, size, and full path.

The original version was written in C using the Windows API (`FindFirstFile`, `FindNextFile`). This Java edition replaces that with Java NIO's `Files.walkFileTree()`, achieving the same recursive traversal behavior while being fully cross-platform.

---

## Features

- **Recursive Directory Traversal** — Searches all nested subdirectories from any root path
- **Partial Name Matching** — Matches any file or folder name containing the query (case-insensitive)
- **Formatted Result Table** — Displays name, size (B / KB / MB), and absolute path
- **Permission-Safe** — Skips inaccessible directories without crashing
- **Search Timer** — Shows total time taken for each search
- **Cross-Platform** — Runs on Windows, Linux, and macOS

---

## Getting Started

### Prerequisites

- Java Development Kit (JDK) **11 or higher**

### Build

```bat
build.bat
```

Or manually on any platform:

```bash
javac -d out -sourcepath src src/com/filesearch/main/Main.java
```

### Run

```bat
run.bat
```

Or manually:

```bash
java -cp out com.filesearch.main.Main
```

---

## Usage

```
╔══════════════════════════════════════╗
║       FILE SEARCH UTILITY v3.0       ║
║         Java Edition (SOLID)         ║
╚══════════════════════════════════════╝

  1. New Search
  2. Exit
```

Enter the root directory path, then type the file or folder name to search for.

### Example Output

```
Path: C:\Users\John
Search query: report

┌─────────────────────────────────────────────────────────────────────┐
│  Search: "report"                                                   │
├──────────────────────┬──────────┬──────────────────────────────────┤
│ Name                 │ Size     │ Full Path                        │
├──────────────────────┼──────────┼──────────────────────────────────┤
│ annual_report.pdf    │ 2.3 MB   │ C:\Users\John\Documents\ann..    │
│ report_2024.docx     │ 450.2 KB │ C:\Users\John\Downloads\rep..    │
│ reports              │ <DIR>    │ C:\Users\John\reports            │
└──────────────────────┴──────────┴──────────────────────────────────┘
  3 result(s) found.  (1.24 seconds)
```

---

## Version History

| Version | Language | Key Changes |
|---|---|---|
| v1.0 | C | Initial implementation using Windows API (`FindFirstFile`, `FindNextFile`) |
| v2.0 | C | Modular separation into `gui`, `search`, and `utils` with header files |
| v3.0 | **Java** | Full rewrite — SOLID principles, clean package structure, cross-platform |




