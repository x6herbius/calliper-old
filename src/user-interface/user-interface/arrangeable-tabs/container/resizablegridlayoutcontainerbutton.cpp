#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QLabel>
#include <QHBoxLayout>

#include "resizablegridlayoutcontainerbutton.h"

namespace UserInterface
{
    ResizableGridLayoutContainerButton::ResizableGridLayoutContainerButton(QWidget *parent)
        : QFrame(parent),
          m_pSelectAction(nullptr),
          m_pMaximiseAction(nullptr),
          m_pCloseAction(nullptr),
          m_iItemID(-1),
          m_pLabel(nullptr)
    {
        initLayout();
        initActions();
    }

    int ResizableGridLayoutContainerButton::itemId() const
    {
        return m_iItemID;
    }

    void ResizableGridLayoutContainerButton::setItemId(int id)
    {
        m_iItemID = id;
    }

    QString ResizableGridLayoutContainerButton::label() const
    {
        return m_pLabel->text();
    }

    void ResizableGridLayoutContainerButton::setLabel(const QString &text)
    {
        m_pLabel->setText(text);
    }

    void ResizableGridLayoutContainerButton::initActions()
    {
        m_pSelectAction = new QAction(tr("&Select"));
        connect(m_pSelectAction, &QAction::triggered, [this]{ emit selectInvoked(m_iItemID); });

        m_pMaximiseAction = new QAction(tr("&Maximise"));
        connect(m_pMaximiseAction, &QAction::triggered, [this]{ emit maximiseInvoked(m_iItemID); });

        m_pCloseAction = new QAction(tr("&Close"));
        connect(m_pCloseAction, &QAction::triggered, [this]{ emit closeInvoked(m_iItemID); });
    }

    void ResizableGridLayoutContainerButton::initLayout()
    {
        QHBoxLayout* l = new QHBoxLayout();
        l->setContentsMargins(0,0,0,0);
        l->setSpacing(0);
        setLayout(l);

        m_pLabel = new QLabel();
        m_pLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        l->addWidget(m_pLabel);

        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        setFrameStyle(QFrame::Panel | QFrame::Raised);
    }

    void ResizableGridLayoutContainerButton::contextMenuEvent(QContextMenuEvent *event)
    {
        QMenu menu(this);

        menu.addAction(m_pSelectAction);
        menu.addAction(m_pMaximiseAction);
        menu.addAction(m_pCloseAction);

        menu.exec(event->globalPos());
    }

    void ResizableGridLayoutContainerButton::mousePressEvent(QMouseEvent *event)
    {
        switch ( event->button() )
        {
            case Qt::LeftButton:
            {
                m_pSelectAction->trigger();
                return;
            }

            case Qt::MiddleButton:
            {
                m_pCloseAction->trigger();
                return;
            }

            default:
                break;
        }

        QFrame::mousePressEvent(event);
    }

    void ResizableGridLayoutContainerButton::mouseDoubleClickEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        m_pMaximiseAction->trigger();
    }
}
