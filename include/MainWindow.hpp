#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class QLabel;
class QCloseEvent;
class QPushButton;
class QTreeView;
class PasswordModel;
class CopyDelegate;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const QString& masterKey, QWidget *parent = nullptr);
    void setPassword(const QString password);
    
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onRemoveClicked();

private:
    const QString& m_masterKey;
    void setupUi();
    void setupConnections();
    int m_closeCount = 0;
    QLabel *m_label;
    QString m_password;
    QPushButton *m_removeButton;
    QPushButton *m_addButton;
    QTreeView *m_treeView;
    PasswordModel* m_passwordModel;
    CopyDelegate* m_delegate;

};

#endif