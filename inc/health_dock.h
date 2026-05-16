#ifndef SENSORHUB_HEALTH_DOCK_H
#define SENSORHUB_HEALTH_DOCK_H

#include <QDockWidget>
#include <QString>

class QTableWidget;
class QLabel;
class QPushButton;

namespace sensorhub {

class HealthMonitor;

class HealthDock : public QDockWidget {
    Q_OBJECT
public:
    explicit HealthDock(HealthMonitor *monitor, QWidget *parent = nullptr);

public slots:
    void refresh();
    void onStaleThresholdChanged(int secs);

private slots:
    void onResetClicked();

private:
    void buildUi();

    HealthMonitor *m_monitor;
    QTableWidget  *m_table;
    QLabel        *m_summary;
    QPushButton   *m_resetBtn;
};

} // namespace sensorhub

#endif // SENSORHUB_HEALTH_DOCK_H
