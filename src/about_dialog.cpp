#include "about_dialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("About");
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("<b>sensorhub-qt</b> v0.1.0"));
    layout->addWidget(new QLabel("Desktop monitoring client for sensorhub IoT backend."));
    layout->addWidget(new QLabel("Built with Qt5 Widgets and Qt5 Network."));
    layout->addWidget(new QLabel("Author: Sairam Santhanam Sukumar"));
    auto* ok = new QPushButton("OK", this);
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(ok);
}
