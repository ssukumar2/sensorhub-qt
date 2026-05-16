#include "notification_banner.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QTimer>

namespace sensorhub {

NotificationBanner::NotificationBanner(QWidget *parent)
    : QWidget(parent),
      m_text(new QLabel(this)),
      m_close(new QToolButton(this)),
      m_hold(new QTimer(this)),
      m_showing(false) {
    setVisible(false);
    setMinimumHeight(28);
    setMaximumHeight(40);
    setAutoFillBackground(true);

    m_text->setWordWrap(true);
    m_text->setTextInteractionFlags(Qt::TextBrowserInteraction);
    m_text->setContentsMargins(8, 4, 8, 4);

    m_close->setText(QStringLiteral("\u2715"));
    m_close->setAutoRaise(true);
    m_close->setToolTip(tr("Dismiss"));

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(4, 0, 4, 0);
    lay->setSpacing(4);
    lay->addWidget(m_text, 1);
    lay->addWidget(m_close, 0, Qt::AlignVCenter);

    m_hold->setSingleShot(true);
    connect(m_hold, &QTimer::timeout, this, &NotificationBanner::onHoldExpired);
    connect(m_close, &QToolButton::clicked, this, &NotificationBanner::onDismissClicked);
}

void NotificationBanner::post(const QString &message, Level level, int holdMs) {
    Item it{message, level, holdMs > 0 ? holdMs : 4000};
    m_queue.enqueue(it);
    if (!m_showing) showNext();
}

void NotificationBanner::clear() {
    m_queue.clear();
    m_hold->stop();
    m_showing = false;
    setVisible(false);
    emit dismissed();
}

void NotificationBanner::showNext() {
    if (m_queue.isEmpty()) {
        m_showing = false;
        setVisible(false);
        emit dismissed();
        return;
    }
    const Item it = m_queue.dequeue();
    m_text->setText(it.message);
    applyStyle(it.level);
    setVisible(true);
    m_showing = true;
    m_hold->start(it.holdMs);
}

void NotificationBanner::applyStyle(Level level) {
    QString bg, fg;
    switch (level) {
        case Info:    bg = QStringLiteral("#1d4e89"); fg = QStringLiteral("#ffffff"); break;
        case Warning: bg = QStringLiteral("#b07c00"); fg = QStringLiteral("#ffffff"); break;
        case Error:   bg = QStringLiteral("#a02020"); fg = QStringLiteral("#ffffff"); break;
    }
    setStyleSheet(QStringLiteral(
        "sensorhub--NotificationBanner { background-color: %1; }"
        "QLabel { color: %2; font-weight: 500; }"
        "QToolButton { color: %2; border: none; padding: 2px 6px; }"
        "QToolButton:hover { background-color: rgba(255,255,255,0.15); }"
    ).arg(bg, fg));
}

void NotificationBanner::onDismissClicked() { m_hold->stop(); showNext(); }
void NotificationBanner::onHoldExpired()    { showNext(); }

} // namespace sensorhub
