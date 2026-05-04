# File Search Utility — C++ / SOLID

A lightweight Windows GUI application (Win32 API + C++17) that searches files
by **name**, **extension**, or **content** — recursively — with SOLID principles
applied throughout.

---

## 📁 Folder Structure

```
file-search-utility/
├── src/
│   ├── main.cpp                          → Entry point — init window + message loop only
│   ├── gui/
│   │   ├── MainWindow.cpp                → Orchestrates controls and user events
│   │   └── ResultsListView.cpp           → Owns and manages the Win32 ListView control
│   ├── search/
│   │   ├── FileNameSearchStrategy.cpp    → Searches files by name (partial, case-insensitive)
│   │   ├── ExtensionSearchStrategy.cpp   → Searches files by extension (.txt, .pdf …)
│   │   └── ContentSearchStrategy.cpp     → Searches inside file content (line-by-line)
│   └── utils/
│       └── FileUtils.cpp                 → Shared helpers: string compare, size formatter
│
├── include/
│   ├── gui/
│   │   ├── IWindow.h                     → GUI abstraction (DIP)
│   │   ├── MainWindow.h                  → Main window declarations + control IDs
│   │   └── ResultsListView.h             → ListView sub-component declaration
│   ├── search/
│   │   ├── ISearchStrategy.h             → Search abstraction (OCP + DIP)
│   │   ├── FileNameSearchStrategy.h
│   │   ├── ExtensionSearchStrategy.h
│   │   └── ContentSearchStrategy.h
│   ├── model/
│   │   └── SearchResult.h                → Plain data struct (SRP)
│   └── utils/
│       └── FileUtils.h                   → Utility namespace declaration (ISP)
│
├── bin/                                  → FileSearch.exe appears here after build
├── build/                                → Object files (intermediate)
├── build.bat                             → MinGW-w64 build script
├── .gitignore
└── README.md
```

---

## ✅ SOLID Principles Map

### S — Single Responsibility Principle
| Class / File | One Responsibility |
|---|---|
| `SearchResult` | Holds data for one matched file — nothing else |
| `FileNameSearchStrategy` | Name-based matching logic only |
| `ResultsListView` | Owns the Win32 ListView control only |
| `MainWindow` | Orchestrates controls and reacts to events |
| `FileUtils` | Stateless string & size helpers |
| `main.cpp` | Creates window + runs message loop — nothing more |

### O — Open/Closed Principle
`ISearchStrategy` lets you add new search modes (e.g. `RegexSearchStrategy`,
`DateSearchStrategy`) **without touching** any existing file.
`MainWindow::buildStrategy()` is the only place that changes.

### L — Liskov Substitution Principle
`FileNameSearchStrategy`, `ExtensionSearchStrategy`, and `ContentSearchStrategy`
all satisfy the `ISearchStrategy` contract fully — any one can replace another
in `MainWindow` without breaking behaviour.

### I — Interface Segregation Principle
- `ISearchStrategy` — 2 methods: `search()` + `name()`
- `IWindow` — 2 methods: `create()` + `handle()`
- `FileUtils` namespace — free functions; callers include only what they use.

No class is forced to implement methods it doesn't need.

### D — Dependency Inversion Principle
- `main.cpp` holds `std::unique_ptr<IWindow>` — not `MainWindow*`
- `MainWindow` holds `std::unique_ptr<ISearchStrategy>` — set at runtime
- Concrete classes are injected; high-level code never `#include`s them directly

---

## 🚀 Build

Requires **MinGW-w64** (`g++`) on your `PATH`.

```bat
build.bat
```

Output: `bin\FileSearch.exe`

---

## 🔍 Search Modes

| Mode | Behaviour |
|------|-----------|
| By Name | Case-insensitive partial match on file name |
| By Extension | Exact extension match — `txt` or `.txt` both work |
| By Content | Line-by-line scan of text files for the query string |
