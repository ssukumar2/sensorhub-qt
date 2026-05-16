#ifndef SENSORHUB_SENSOR_HEALTH_H
#define SENSORHUB_SENSOR_HEALTH_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QHash>
#include <QVector>

namespace sensorhub {

struct SensorHealth {
    QString    id;
    QString    name;
    QString    location;
    QDateTime  firstSeen;
    QDateTime  lastSeen;
    int        consecutiveMisses = 0;
    int        totalSamples      = 0;
    double     avgLatencyMs      = 0.0;
    bool       stale             = false;

    qint64 uptimeSecs() const {
        if (!firstSeen.isValid() || !lastSeen.isValid()) return 0;
        return firstSeen.secsTo(lastSeen);
    }
};

class HealthMonitor : public QObject {
    Q_OBJECT
public:
    explicit HealthMonitor(QObject *parent = nullptr);

    void setStaleAfterSecs(int secs);
    int  staleAfterSecs() const { return m_staleAfterSecs; }

    QVector<SensorHealth> snapshot() const;
    SensorHealth          forId(const QString &id) const;
    int                   sensorCount() const { return m_records.size(); }
    int                   staleCount()  const;

public slots:
    void recordObservation(const QVector<QStringList> &sensors);
    void recordLatency(qint64 latencyMs);
    void evaluateStaleness();
    void reset();

signals:
    void sensorAppeared(const QString &id, const QString &name);
    void sensorWentStale(const QString &id, const QString &name);
    void sensorRecovered(const QString &id, const QString &name);
    void snapshotChanged();

private:
    QHash<QString, SensorHealth> m_records;
    int                          m_staleAfterSecs;
};

} // namespace sensorhub

#endif // SENSORHUB_SENSOR_HEALTH_H
