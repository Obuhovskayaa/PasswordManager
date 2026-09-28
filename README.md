# Qt Password Manager (under development)

 A prototype password manager written in C++ and Qt (Widgets).

* **Custom delegates, Model/View architecture** in Qt. Instead of creating heavyweight `QPushButton` widgets for each table cell (which leads to excessive memory consumption and rendering delays), a lightweight custom **`QStyledItemDelegate`** is used. The action icons (“Copy password”) are drawn manually in SVG format inside the `paint()` method of the delegate, and the hover and click states are dynamically controlled using event filters.
* A launch procedure with state control (`LoginDialog` / `SetupDialog`) has been implemented; it verifies access using master password hashing and parameterized SQL queries (`bindValue`)
* **Cryptography (AES-256 and SHA-256)**: the database security is ensured using a separate 24‑character **master key**, which is used to encrypt all fields of SQLite tables on the fly using the `QAESEncryption` library. To avoid storing the master key in plain text in a file, it is encrypted using the user’s 4–6‑character **PIN code** and saved in `QSettings` along with SHA‑256 hashes. This architecture ensures simple daily authentication using a PIN code, protects data even in the event of a configuration file leak, and provides the ability to restore access using the original master key if the user forgets their PIN code.

- **Memory security (runtime leaks)**: currently, master keys and decrypted passwords are processed using standard `QString` containers. This results in data remaining in unencrypted form in RAM during program execution, and copies of it may be saved in the system swap file. In future updates, we will replace them with secure memory buffers that zero out confidential data immediately after use and prevent page swapping.

## TODO

- **Protected data buffers during execution**: preventing leaks of decrypted data by using special protected containers during program execution.
- **Interface refinement**: changing the structure of the main window, removing hardcoded dimensions, and aligning style sheets with a unified design system.
- **Dynamic search and filtering**: add a search bar for dynamic filtering of service records using the QSortFilterProxyModel from Qt.