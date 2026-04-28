#include "settings_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>

SettingsDialog::SettingsDialog(const QString& currentUrl, int refreshInterval, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Settings");
    auto* form = new QFormLayout;

    urlEdit_ = new QLineEdit(currentUrl, this);
    form->addRow("Backend URL:", urlEdit_);

    intervalSpin_ = new QSpinBox(this);
    intervalSpin_->setRange(1, 60);
    intervalSpin_->setValue(refreshInterval);
    intervalSpin_->setSuffix(" sec");
    form->addRow("Refresh interval:", intervalSpin_);

    auto* buttons = new QHBoxLayout;
    auto* ok = new QPushButton("OK", this);
    auto* cancel = new QPushButton("Cancel", this);
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
    buttons->addWidget(ok);
    buttons->addWidget(cancel);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addLayout(buttons);
}

QString SettingsDialog::backendUrl() const { return urlEdit_->text(); }
int SettingsDialog::refreshInterval() const { return intervalSpin_->value(); }
