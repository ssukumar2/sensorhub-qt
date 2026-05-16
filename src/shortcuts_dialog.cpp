#include "shortcuts_dialog.h"
#include "keyboard_shortcuts.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QDialogButtonBox>

namespace sensorhub {

ShortcutsDialog::ShortcutsDialog(KeyboardShortcuts *registry, QWidget *parent)
    : QDialog(parent), m_registry(registry), m_table(new QTableWidget(this)) {
    setWindowTitle(tr("Keyboard shortcuts"));
    resize(480, 360);

    m_table->setColumnCount(2);
    m_table->setHorizontalHeaderLabels(QStringList{tr("Action"), tr("Shortcut")});
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->setSortingEnabled(true);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(m_table, 1);
    lay->addWidget(buttons);

    populate();
}

void ShortcutsDialog::populate() {
    if (!m_registry) return;
    const QStringList ids = m_registry->ids();
    m_table->setSortingEnabled(false);
    m_table->setRowCount(ids.size());
    int row = 0;
    for (const QString &id : ids) {
        QTableWidgetItem *a = new QTableWidgetItem(m_registry->descriptionFor(id));
        QTableWidgetItem *b = new QTableWidgetItem(m_registry->keysFor(id).toString(QKeySequence::NativeText));
        a->setData(Qt::UserRole, id);
        m_table->setItem(row, 0, a);
        m_table->setItem(row, 1, b);
        ++row;
    }
    m_table->setSortingEnabled(true);
    m_table->sortItems(0, Qt::AscendingOrder);
}

} // namespace sensorhub
