#ifndef SENSORHUB_NOTIFICATION_BANNER_H
#define SENSORHUB_NOTIFICATION_BANNER_H

#include <QWidget>
#include <QString>
#include <QQueue>

class QLabel;
class QToolButton;
class QTimer;

namespace sensorhub {

class NotificationBanner : public QWidget {
    Q_OBJECT
public:
    enum Level { Info, Warning, Error };
    Q_ENUM(Level)

    explicit NotificationBanner(QWidget *parent = nullptr);

    void post(const QString &message, Level level = Info, int holdMs = 4000);
    void clear();

signals:
    void dismissed();

private slots:
    void onDismissClicked();
    void onHoldExpired();

private:
    struct Item { QString message; Level level; int holdMs; };

    void showNext();
    void applyStyle(Level level);

    QLabel       *m_text;
    QToolButton  *m_close;
    QTimer       *m_hold;
    QQueue<Item>  m_queue;
    bool          m_showing;
};

} // namespace sensorhub

#endif // SENSORHUB_NOTIFICATION_BANNER_H
