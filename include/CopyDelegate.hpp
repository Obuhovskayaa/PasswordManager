#ifndef COPY_DELEGATE_HPP
#define COPY_DELEGATE_HPP

#include <QStyledItemDelegate>
#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>
#include <QModelIndex>
#include <QPixmap>
#include <QTimer>

class QPainter;
class QTimer;

class CopyDelegate : public QStyledItemDelegate {
public:
    explicit CopyDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(
        QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index
    ) override;

    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex &index) const override;
    void setView(QAbstractItemView* view);
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool eventFilter(QObject* watched, QEvent *event) override;

private:
    QRect copyIconRect(const QStyleOptionViewItem &option) const;
    QPoint mousePos(const QStyleOptionViewItem &option) const;
    
    mutable bool m_wasOverIcon;
    mutable QModelIndex m_lastIndex;
    QModelIndex m_pressedIndex;
    bool m_copyPressed = false;
    double m_rippleRadius;
    double m_rippleOpacity;
    QTimer *m_rippleTimer;

};

#endif