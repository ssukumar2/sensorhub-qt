#include "mainwindow.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) 
{
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    statusLabel_ = new QLabel("connecting...", this);
    layout->addWidget(statusLabel_);

    table_ = new QTableWidget(0, 3, this);
    table_->setHorizontalHeaderLabels({"ID", "Name", "Location"});
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(table_);

    setCentralWidget(central);

    net_ = new QNetworkAccessManager(this);
    connect(net_, &QNetworkAccessManager::finished, this, &MainWindow::onSensorsReply);

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::refreshSensors);
    timer_->start(5000);
    refreshSensors();
}

void MainWindow::refreshSensors() {
    net_->get(QNetworkRequest(QUrl(backendUrl_ + "/sensors")));
}

void MainWindow::onSensorsReply(QNetworkReply* reply) 
{
    if (reply->error() != QNetworkReply::NoError) 
    {
        statusLabel_->setText("offline");
        statusLabel_->setStyleSheet("color: red;");
        reply->deleteLater();
        return;
    }

    statusLabel_->setText("connected");
    statusLabel_->setStyleSheet("color: green;");

    auto doc = QJsonDocument::fromJson(reply->readAll());
    auto arr = doc.array();
    table_->setRowCount(arr.size());

    for (int i = 0; i < arr.size(); ++i) 
    {
        auto obj = arr[i].toObject();
        table_->setItem(i, 0, new QTableWidgetItem(QString::number(obj["id"].toInt())));
        table_->setItem(i, 1, new QTableWidgetItem(obj["name"].toString()));
        table_->setItem(i, 2, new QTableWidgetItem(obj["location"].toString()));
    }
    
    reply->deleteLater();
}