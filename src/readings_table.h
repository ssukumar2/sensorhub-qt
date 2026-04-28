#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ReadingsTable : public QWidget {
    Q_OBJECT
public:
    explicit ReadingsTable(QWidget* parent = nullptr);
    void loadForSensor(int sensorId, const QString& backendUrl);
private slots:
    void onReply(QNetworkReply* reply);
private:
    QTableWidget* table_;
    QNetworkAccessManager* net_;
};
