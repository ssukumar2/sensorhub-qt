#include "readings_table.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHeaderView>
#include <QLabel>

ReadingsTable::ReadingsTable(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel("Readings", this);
    label->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(label);

    table_ = new QTableWidget(0, 3, this);
    table_->setHorizontalHeaderLabels({"ID", "Value", "Unit"});
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(table_);

    net_ = new QNetworkAccessManager(this);
    connect(net_, &QNetworkAccessManager::finished, this, &ReadingsTable::onReply);
}

void ReadingsTable::loadForSensor(int sensorId, const QString& backendUrl) {
    QString url = QString("%1/sensors/%2/readings").arg(backendUrl).arg(sensorId);
    net_->get(QNetworkRequest(QUrl(url)));
}

void ReadingsTable::onReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }
    auto doc = QJsonDocument::fromJson(reply->readAll());
    auto arr = doc.array();
    int count = qMin(arr.size(), 50);
    table_->setRowCount(count);
    for (int i = 0; i < count; ++i) {
        auto obj = arr[arr.size() - count + i].toObject();
        table_->setItem(i, 0, new QTableWidgetItem(QString::number(obj["id"].toInt())));
        table_->setItem(i, 1, new QTableWidgetItem(QString::number(obj["value"].toDouble(), 'f', 2)));
        table_->setItem(i, 2, new QTableWidgetItem(obj["unit"].toString()));
    }
    reply->deleteLater();
}
