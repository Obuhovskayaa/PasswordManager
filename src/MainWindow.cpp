
#include "MainWindow.hpp"
#include "PasswordModel.hpp"
#include "Entry.hpp"
#include "CopyDelegate.hpp"
#include "DatabaseManager.hpp"
#include <QTreeView>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QList>
#include <QItemSelection>
#include <QMessageBox>
#include <QPushButton>
#include <QClipboard>
#include <QApplication>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(const QString& masterKey, QWidget *parent) 
    : QMainWindow(parent), m_masterKey(masterKey)
{
    setupUi();
    setupConnections();
}

void MainWindow::setupUi() {

    QWidget *central = new QWidget();
    setCentralWidget(central);

    if (!DatabaseManager::instance().connectToDatabase(m_masterKey)) {
        QMessageBox::critical(nullptr, "Error", "Could not open database");
        return;
    }

    m_passwordModel = new PasswordModel(this);

    m_treeView = new QTreeView();
    m_treeView->setModel(m_passwordModel);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    m_delegate = new CopyDelegate();
    m_treeView->setItemDelegate(m_delegate);
    m_delegate->setView(m_treeView);
    m_treeView->setMouseTracking(true);
    m_treeView->viewport()->setMouseTracking(true);

    m_addButton = new QPushButton("add entry");
    m_removeButton = new QPushButton("remove entry");
    m_removeButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_treeView);

    resize(400, 300);
    
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_closeCount < 3) {
        m_closeCount++;
        event->ignore();
    } else {
        m_closeCount = 0;
        event->accept();
    }
}

void MainWindow::onRemoveClicked() {
    QModelIndex index = m_treeView->currentIndex();

    if (!index.isValid() || PasswordModel::unpackType(index.internalId()) != PasswordModel::ItemType::Service) {
        return;
    }

    auto reply = QMessageBox::question(
        this, "Removing", "Remove an entry?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_passwordModel->removeEntry(index);
    }
}

void MainWindow::setupConnections() {

    connect(m_treeView->selectionModel(),
     &QItemSelectionModel::selectionChanged,
      [this](const QItemSelection &selected)
    {
        if (selected.isEmpty()) {
             m_removeButton->setEnabled(false);
            return;
        }
        QModelIndex index = selected.indexes().first();

        bool isService = (PasswordModel::unpackType(index.internalId()) == PasswordModel::ItemType::Service);

        m_removeButton->setEnabled(isService);
    });

    // connect(m_treeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &index){

    //     if (index.isValid() && index.internalId() != (quintptr(-1))) {

    //         QString text = index.data(Qt::DisplayRole).toString();

    //          QApplication::clipboard()->setText(text);
    //     }
    // });

    connect(m_treeView, &QTreeView::customContextMenuRequested, [this](const QPoint &pos)
    {
        QModelIndex index = m_treeView->indexAt(pos);
        if (!index.isValid()) {
            return;
        }

        QMenu menu(this);

        if (index.internalId() != (quintptr(-1))) {
            QAction *copyAction = menu.addAction("Copy");

            connect(copyAction, &QAction::triggered, [index](){
                QApplication::clipboard()->setText(index.data().toString());
            });

            QAction *editAction = menu.addAction("Edit");

        } 
    });

    connect(m_addButton, &QPushButton::clicked, m_passwordModel, &PasswordModel::addEntry);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);

}

void MainWindow::setPassword(const QString password) {
    m_password = password;
    qDebug() << "password is set";
}
