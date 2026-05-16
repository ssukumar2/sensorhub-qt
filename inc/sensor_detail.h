#pragma once
#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SensorDetail : public QWidget {
    Q_OBJECT
public:
    explicit SensorDetail(QWidget* parent = nullptr);
    void loadSensor(int sensorId, const QString& backendUrl);
private slots:
    void onReply(QNetworkReply* reply);
private:
    QLabel* nameLabel_;
    QLabel* locationLabel_;
    QLabel* idLabel_;
    QLabel* lastValueLabel_;
    QNetworkAccessManager* net_;
};
