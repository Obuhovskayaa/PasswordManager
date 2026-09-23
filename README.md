# Qt Password Manager (under development)

 A prototype password manager written in C++ and Qt (Widgets).

* **Custom delegates, Model/View architecture** in Qt. Instead of creating heavyweight `QPushButton` widgets for each table cell (which leads to excessive memory consumption and rendering delays), a lightweight custom **`QStyledItemDelegate`** is used. The action icons (“Copy password”) are drawn manually in SVG format inside the `paint()` method of the delegate, and the hover and click states are dynamically controlled using event filters.
* A launch procedure with state control (`LoginDialog` / `SetupDialog`) has been implemented; it verifies access using master password hashing and parameterized SQL queries (`bindValue`)

## TODO

- **Full database encryption**: transitioning from plaintext columns to encrypting all confidential user data (logins, passwords, notes) in SQLite.
- **Protected data buffers during execution**: preventing leaks of decrypted data by using special protected containers during program execution.
- **Interface refinement**: changing the structure of the main window, removing hardcoded dimensions, and aligning style sheets with a unified design system.
- **Dynamic search and filtering**: add a search bar for dynamic filtering of service records using the QSortFilterProxyModel from Qt.