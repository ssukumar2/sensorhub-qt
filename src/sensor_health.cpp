#include "sensor_health.h"

namespace sensorhub {

HealthMonitor::HealthMonitor(QObject *parent)
    : QObject(parent), m_staleAfterSecs(30) {}

void HealthMonitor::setStaleAfterSecs(int secs) {
    if (secs > 0) m_staleAfterSecs = secs;
}

int HealthMonitor::staleCount() const {
    int n = 0;
    for (auto it = m_records.constBegin(); it != m_records.constEnd(); ++it) {
        if (it.value().stale) ++n;
    }
    return n;
}

QVector<SensorHealth> HealthMonitor::snapshot() const {
    QVector<SensorHealth> out;
    out.reserve(m_records.size());
    for (auto it = m_records.constBegin(); it != m_records.constEnd(); ++it) {
        out.append(it.value());
    }
    return out;
}

SensorHealth HealthMonitor::forId(const QString &id) const {
    return m_records.value(id);
}

void HealthMonitor::recordObservation(const QVector<QStringList> &sensors) {
    const QDateTime now = QDateTime::currentDateTime();
    QHash<QString, bool> seenNow;
    for (const QStringList &row : sensors) {
        if (row.size() < 1) continue;
        const QString id       = row.value(0);
        const QString name     = row.value(1);
        const QString location = row.value(2);
        if (id.isEmpty()) continue;
        seenNow.insert(id, true);

        auto it = m_records.find(id);
        if (it == m_records.end()) {
            SensorHealth h;
            h.id = id; h.name = name; h.location = location;
            h.firstSeen = now; h.lastSeen = now; h.totalSamples = 1;
            m_records.insert(id, h);
            emit sensorAppeared(id, name);
        } else {
            const bool wasStale = it.value().stale;
            it.value().name = name;
            it.value().location = location;
            it.value().lastSeen = now;
            it.value().consecutiveMisses = 0;
            it.value().totalSamples += 1;
            if (wasStale) {
                it.value().stale = false;
                emit sensorRecovered(id, name);
            }
        }
    }
    for (auto it = m_records.begin(); it != m_records.end(); ++it) {
        if (!seenNow.contains(it.key())) it.value().consecutiveMisses += 1;
    }
    evaluateStaleness();
    emit snapshotChanged();
}

void HealthMonitor::recordLatency(qint64 latencyMs) {
    if (latencyMs < 0 || m_records.isEmpty()) return;
    const double alpha = 0.25;
    for (auto it = m_records.begin(); it != m_records.end(); ++it) {
        if (it.value().avgLatencyMs <= 0.0) {
            it.value().avgLatencyMs = static_cast<double>(latencyMs);
        } else {
            it.value().avgLatencyMs =
                (1.0 - alpha) * it.value().avgLatencyMs +
                alpha * static_cast<double>(latencyMs);
        }
    }
}

void HealthMonitor::evaluateStaleness() {
    const QDateTime now = QDateTime::currentDateTime();
    for (auto it = m_records.begin(); it != m_records.end(); ++it) {
        if (!it.value().lastSeen.isValid()) continue;
        const qint64 age = it.value().lastSeen.secsTo(now);
        const bool nowStale = age >= m_staleAfterSecs;
        if (nowStale && !it.value().stale) {
            it.value().stale = true;
            emit sensorWentStale(it.value().id, it.value().name);
        }
    }
}

void HealthMonitor::reset() {
    m_records.clear();
    emit snapshotChanged();
}

} // namespace sensorhub
