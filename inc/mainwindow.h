#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace sensorhub {
class AppContext;
class NotificationBanner;
class HealthDock;
class KeyboardShortcuts;
}

class MainWindow : public QMainWindow 
{
    Q_OBJECT
public:
    void setAppContext(sensorhub::AppContext *ctx);

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

private:
    sensorhub::AppContext         *m_appCtx        = nullptr;
    sensorhub::NotificationBanner *m_banner        = nullptr;
    sensorhub::HealthDock         *m_healthDock    = nullptr;
    sensorhub::KeyboardShortcuts  *m_shortcuts     = nullptr;

private slots:
    void onShortcutTriggered(const QString &id);
    void onSensorWentStale(const QString &id, const QString &name);
    void onSensorRecovered(const QString &id, const QString &name);

private:
    void installViewMenuEntries();
};