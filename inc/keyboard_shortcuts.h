#ifndef SENSORHUB_KEYBOARD_SHORTCUTS_H
#define SENSORHUB_KEYBOARD_SHORTCUTS_H

#include <QObject>
#include <QKeySequence>
#include <QHash>
#include <QString>

class QShortcut;
class QWidget;

namespace sensorhub {

class KeyboardShortcuts : public QObject {
    Q_OBJECT
public:
    explicit KeyboardShortcuts(QWidget *parentWidget);

    QShortcut *bind(const QString &id, const QString &description, const QKeySequence &keys);

    QKeySequence keysFor(const QString &id) const;
    QString      descriptionFor(const QString &id) const;
    QStringList  ids() const;

signals:
    void triggered(const QString &id);

private:
    struct Entry { QString description; QKeySequence keys; QShortcut *shortcut; };
    QWidget               *m_parent;
    QHash<QString, Entry>  m_entries;
};

} // namespace sensorhub

#endif // SENSORHUB_KEYBOARD_SHORTCUTS_H
