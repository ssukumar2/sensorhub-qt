#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MainWindow : public QMainWindow 
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void refreshSensors();
    void onSensorsReply(QNetworkReply* reply);

private:
    QTableWidget* table_;
    QLabel* statusLabel_;
    QTimer* timer_;
    QNetworkAccessManager* net_;
    QString backendUrl_ = "http://localhost:8000";
};