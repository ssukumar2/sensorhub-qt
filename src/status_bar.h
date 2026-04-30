#pragma once
#include <QStatusBar>
#include <QLabel>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class StatusBarWidget : public QStatusBar {
    Q_OBJECT
public:
    explicit StatusBarWidget(const QString& backendUrl, QWidget* parent = nullptr);
    void setBackendUrl(const QString& url);

private slots:
    void checkHealth();
    void onHealthReply(QNetworkReply* reply);

private:
    QLabel* statusIcon_;
    QLabel* statusText_;
    QLabel* sensorCount_;
    QTimer* timer_;
    QNetworkAccessManager* net_;
    QString backendUrl_;
};
