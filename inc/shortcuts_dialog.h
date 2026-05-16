#ifndef SENSORHUB_SHORTCUTS_DIALOG_H
#define SENSORHUB_SHORTCUTS_DIALOG_H

#include <QDialog>

class QTableWidget;

namespace sensorhub {

class KeyboardShortcuts;

class ShortcutsDialog : public QDialog {
    Q_OBJECT
public:
    explicit ShortcutsDialog(KeyboardShortcuts *registry, QWidget *parent = nullptr);

private:
    void populate();

    KeyboardShortcuts *m_registry;
    QTableWidget      *m_table;
};

} // namespace sensorhub

#endif // SENSORHUB_SHORTCUTS_DIALOG_H
