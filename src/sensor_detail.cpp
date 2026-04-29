#include "sensor_detail.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFont>

SensorDetail::SensorDetail(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto titleFont = QFont();
    titleFont.setBold(true);
    titleFont.setPointSize(12);

    idLabel_ = new QLabel("ID: -", this);
    nameLabel_ = new QLabel("Name: -", this);
    locationLabel_ = new QLabel("Location: -", this);
    lastValueLabel_ = new QLabel("Last value: -", this);

    auto* title = new QLabel("Sensor Detail", this);
    title->setFont(titleFont);
    layout->addWidget(title);
    layout->addWidget(idLabel_);
    layout->addWidget(nameLabel_);
    layout->addWidget(locationLabel_);
    layout->addWidget(lastValueLabel_);
    layout->addStretch();

    net_ = new QNetworkAccessManager(this);
    connect(net_, &QNetworkAccessManager::finished, this, &SensorDetail::onReply);
}

void SensorDetail::loadSensor(int sensorId, const QString& backendUrl) {
    net_->get(QNetworkRequest(QUrl(QString("%1/sensors/%2").arg(backendUrl).arg(sensorId))));
}

void SensorDetail::onReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }
    auto obj = QJsonDocument::fromJson(reply->readAll()).object();
    idLabel_->setText(QString("ID: %1").arg(obj["id"].toInt()));
    nameLabel_->setText(QString("Name: %1").arg(obj["name"].toString()));
    locationLabel_->setText(QString("Location: %1").arg(obj["location"].toString()));
    reply->deleteLater();
}
