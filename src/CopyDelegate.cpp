#include "CopyDelegate.hpp"
#include "PasswordModel.hpp"
#include <QPainter>
#include <QAbstractItemView>
#include <QDebug>
#include <QtSvg/QSvgRenderer>
#include <QMetaObject>
#include <QLineEdit>
#include <QAction>
#include <QIcon>


CopyDelegate::CopyDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

    m_rippleTimer = new QTimer(this);

    connect(m_rippleTimer, &QTimer::timeout, [this]()
            {
        if (m_copyPressed) {
            if (m_rippleRadius < 9) {
                m_rippleRadius += 0.3;
            }
        } else {
            m_rippleOpacity -= 0.1;
            if (m_rippleOpacity <= 0) {
                m_rippleTimer->stop();
                m_rippleRadius = 0;
            }
        }
        auto *view = qobject_cast<QAbstractItemView*>(this->parent());
        if (view) {
             view->update(m_pressedIndex);
        } });
}
void CopyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    bool isField = PasswordModel::unpackType(index.internalId()) != PasswordModel::ItemType::Service;
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    if (!isField) {
        if (m_copyPressed) {
            opt.state &= ~QStyle::State_MouseOver;
            opt.state &= ~QStyle::State_Selected;
        }
        QStyledItemDelegate::paint(painter, opt, index);
        return;
    }
    QRect clickRect = copyIconRect(opt);
    bool mouseOverRow = (opt.state & QStyle::State_MouseOver);
    bool mouseOverIcon = mouseOverRow && clickRect.contains(mousePos(opt)); //(index == m_lastIndex && m_wasOverIcon);

    if (mouseOverIcon || m_copyPressed) {
        opt.state &= ~QStyle::State_MouseOver;
    }

    opt.rect.setRight(opt.rect.right() - 24);
    QStyledItemDelegate::paint(painter, opt, index);

    bool thisPressed = (m_copyPressed && index == m_pressedIndex);
    bool iconActive = false;
    
    // if (index == m_pressedIndex && m_rippleRadius > 0) {
    //     painter->save();
    //     painter->setRenderHint(QPainter::Antialiasing);
    //     painter->setBrush(QColor(200, 200, 200, 150));
    //     painter->setPen(Qt::NoPen);
    //     painter->drawEllipse(clickRect.center(), (int)m_rippleRadius, (int)m_rippleRadius);
    //     painter->restore();
    // }
    painter->save();

    if (thisPressed) {
        iconActive = true;
    } else if (!m_copyPressed && mouseOverIcon) {
        iconActive = true;
    }
    painter->setOpacity(iconActive ? 1.0 : 0.6);
    QRect iconRect(0, 0, 16, 16);
    iconRect.moveCenter(clickRect.center());

    static QSvgRenderer rendererNormal(QString(":/icons/copy-outline.svg"));
    static QSvgRenderer rendererFilled(QString(":/icons/copy.svg"));

    if (thisPressed) {
        rendererFilled.render(painter, iconRect);
    } else {
        rendererNormal.render(painter, iconRect);
    }

    // QIcon copyIcon = QApplication::style()->standardIcon(QStyle::SP_DriveFDIcon);
    // copyIcon.paint(painter, clickRect);

    painter->restore();
}


bool CopyDelegate::editorEvent(
    QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (m_copyPressed)
        {
            return true;
        }

        bool indexChanged = index != m_lastIndex;
        bool isField = PasswordModel::unpackType(index.internalId()) != PasswordModel::ItemType::Service;
        bool wasField = PasswordModel::unpackType(m_lastIndex.internalId()) != PasswordModel::ItemType::Service;
        bool isOverIcon = isField && copyIconRect(option).contains(mouseEvent->pos());
        bool overIconChanged = isOverIcon != (indexChanged ? false : m_wasOverIcon);

        // qDebug()
        //     << "=== Beginning ==="
        //     << "\nindexChanged: " << indexChanged
        //     << "\nisField: " << isField
        //     << "\nwasField: " << wasField
        //     << "\nisOverIcon: " << isOverIcon
        //     << "\noverIconChanged: " << overIconChanged
        //     << "\nindex: " << index.row()
        //     << "\nm_lastIndex: " << m_lastIndex.row();

        if (indexChanged || overIconChanged)
        {
            QAbstractItemView *view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));

            if (isField)
            {
                view->viewport()->update(option.rect);
                // qDebug() << "\ncurrent rect updated";
            }
            if (indexChanged)
            {
                if (m_lastIndex.isValid() && wasField)
                {
                    view->viewport()->update(view->visualRect(m_lastIndex));
                    // qDebug() << "\nlast rect updated";
                }
            }
        }

        m_wasOverIcon = isOverIcon;
        m_lastIndex = index;
        // qDebug() << "\n===End of cycle===";

        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if (PasswordModel::unpackType(index.internalId()) != PasswordModel::ItemType::Service)
            {
                if (copyIconRect(option).contains(mouseEvent->pos()))
                {
                    m_copyPressed = true;
                    m_pressedIndex = index;
                    m_rippleRadius = 0.0;
                    m_rippleOpacity = 1.0;
                    m_rippleTimer->start(16);
                    return true;
                }
            }
        }
    }
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            //qDebug() << "m_copyPressed: " << m_copyPressed;
            if (m_copyPressed)
            {
                //qDebug() << "index row: " << index.row();
                bool releasedOverIcon = index == m_pressedIndex && copyIconRect(option).contains(mouseEvent->pos());
                //qDebug() << "releasedOverIcon: " << releasedOverIcon;
                if (releasedOverIcon) {
                    QApplication::clipboard()->setText(index.data(Qt::DisplayRole).toString());
                }
                return true;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void CopyDelegate::setView(QAbstractItemView *view)
{
    if (view && view->viewport())
    {
        view->viewport()->installEventFilter(this);
    }
}

bool CopyDelegate::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Leave)
    {
        if (m_lastIndex.isValid() && PasswordModel::unpackType(m_lastIndex.internalId()) != PasswordModel::ItemType::Service)
        {
            QAbstractItemView *view = qobject_cast<QAbstractItemView *>(watched->parent());
            if (view)
            {
                view->viewport()->update(view->visualRect(m_lastIndex));
            }
            m_wasOverIcon = false;
            m_copyPressed = false;
            m_pressedIndex = QModelIndex();
            m_lastIndex = QModelIndex();
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton && m_copyPressed) {
            QMetaObject::invokeMethod(this, [this, watched] {
                m_copyPressed = false;
                m_wasOverIcon = false;
                QAbstractItemView *view = qobject_cast<QAbstractItemView*>(watched->parent());
                view->update(m_pressedIndex);
                m_pressedIndex = QModelIndex();
            }, Qt::QueuedConnection);
        }
    }
    QLineEdit *editor = qobject_cast<QLineEdit*>(watched);
    if (editor && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
            emit commitData(editor);
            emit closeEditor(editor);
            return true;
        }
    }
    return QStyledItemDelegate::eventFilter(watched, event);
}

void CopyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *line = qobject_cast<QLineEdit*>(editor);
    if (line) {
        line->setText(text);
    }
}

QWidget* CopyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    editor->setStyleSheet(
        "QLineEdit { "
        "   border: none; "
        "   background: #fdfff5; "
        "}"
    );
    QAction *cancelAction = editor->addAction(QIcon(":/icons/return.svg"), QLineEdit::TrailingPosition);
    cancelAction->setToolTip("Cancel changes");
    
    connect(cancelAction, &QAction::triggered, this, [this, editor]() {
        const_cast<CopyDelegate*>(this)->closeEditor(editor);
    });
    editor->installEventFilter(const_cast<CopyDelegate*>(this));
    return editor;
}

void CopyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QLineEdit* line = qobject_cast<QLineEdit*>(editor);
    if (line) {
        model->setData(index, line->text(), Qt::EditRole);
    }
}

void CopyDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
}

QRect CopyDelegate::copyIconRect(const QStyleOptionViewItem &option) const
{
    int side = 20;
    int top = option.rect.top() + (option.rect.height() - side) / 2;
    int left = option.rect.right() - side - 2;
    return QRect(left, top, side, side);
}

QPoint CopyDelegate::mousePos(const QStyleOptionViewItem &option) const
{
    QAbstractItemView *view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
    QPoint mousePos = view->viewport()->mapFromGlobal(QCursor::pos());
    return mousePos;
}