#include "status_bar.h"
#include <QJsonDocument>
#include <QJsonObject>

StatusBarWidget::StatusBarWidget(const QString& backendUrl, QWidget* parent)
    : QStatusBar(parent), backendUrl_(backendUrl) {
    statusIcon_ = new QLabel("●", this);
    statusIcon_->setStyleSheet("color: gray; font-size: 16px;");
    statusText_ = new QLabel("checking...", this);
    sensorCount_ = new QLabel("", this);

    addWidget(statusIcon_);
    addWidget(statusText_);
    addPermanentWidget(sensorCount_);

    net_ = new QNetworkAccessManager(this);
    connect(net_, &QNetworkAccessManager::finished, this, &StatusBarWidget::onHealthReply);

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &StatusBarWidget::checkHealth);
    timer_->start(3000);
    checkHealth();
}

void StatusBarWidget::setBackendUrl(const QString& url) {
    backendUrl_ = url;
}

void StatusBarWidget::checkHealth() {
    net_->get(QNetworkRequest(QUrl(backendUrl_ + "/health")));
}

void StatusBarWidget::onHealthReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        statusIcon_->setStyleSheet("color: red; font-size: 16px;");
        statusText_->setText("offline");
        sensorCount_->setText("");
    } else {
        statusIcon_->setStyleSheet("color: green; font-size: 16px;");
        statusText_->setText("connected");
        auto doc = QJsonDocument::fromJson(reply->readAll());
        auto obj = doc.object();
        if (obj.contains("sensors"))
            sensorCount_->setText(QString("%1 sensors").arg(obj["sensors"].toInt()));
    }
    reply->deleteLater();
}
