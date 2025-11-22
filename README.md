# Book Library Management System

Qt6 desktop app for managing a book library. C++ university project.

## Features

- Add, edit, delete, search books
- Undo operations
- Shopping basket with export (CSV/HTML)
- Multiple GUI views (list, table, visual)
- File persistence
- Genre statistics

## Tech Stack

- Qt 6.9.0
- C++17
- MSVC 2022

## Build & Run

1. Open `lab_qt.sln` in Visual Studio 2022
2. Build (Ctrl+Shift+B)
3. Run (F5)

Tests run automatically on startup.

## File Structure

```
├── domain.{h,cpp}     # Book entity
├── repo.{h,cpp}       # Data layer (file/memory)
├── service.{h,cpp}    # Business logic
├── basket.{h,cpp}     # Basket operations
├── gui.h              # Main interface
├── tests.{h,cpp}      # Unit tests
└── books.txt          # Data storage
```

## Design Patterns

- Repository (FileRepo/InMemoryRepo)
- Observer (basket updates)
- Command (undo system)
- MVC (Qt models)

## Genres

`SF`, `romance`, `history`, `drama`, `action`, `fantasy`, `YA`

## License

Academic project - Cluj-Napoca, Romania
