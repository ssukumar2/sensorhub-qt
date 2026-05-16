#include "keyboard_shortcuts.h"

#include <QShortcut>
#include <QWidget>

namespace sensorhub {

KeyboardShortcuts::KeyboardShortcuts(QWidget *parentWidget)
    : QObject(parentWidget), m_parent(parentWidget) {}

QShortcut *KeyboardShortcuts::bind(const QString &id,
                                   const QString &description,
                                   const QKeySequence &keys) {
    if (id.isEmpty() || keys.isEmpty() || m_parent == nullptr) return nullptr;
    if (m_entries.contains(id)) return m_entries.value(id).shortcut;
    QShortcut *sc = new QShortcut(keys, m_parent);
    sc->setContext(Qt::WindowShortcut);
    connect(sc, &QShortcut::activated, this, [this, id]() { emit triggered(id); });
    m_entries.insert(id, Entry{description, keys, sc});
    return sc;
}

QKeySequence KeyboardShortcuts::keysFor(const QString &id) const { return m_entries.value(id).keys; }
QString      KeyboardShortcuts::descriptionFor(const QString &id) const { return m_entries.value(id).description; }

QStringList KeyboardShortcuts::ids() const {
    QStringList out = m_entries.keys();
    out.sort();
    return out;
}

} // namespace sensorhub
