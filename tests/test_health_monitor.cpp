#include <QtTest/QtTest>
#include <QObject>
#include <QSignalSpy>
#include <QThread>

#include "sensor_health.h"

using sensorhub::HealthMonitor;
using sensorhub::SensorHealth;

class TestHealthMonitor : public QObject {
    Q_OBJECT
private slots:
    void emitsAppearedOnFirstObservation();
    void marksSensorStaleAfterThreshold();
    void recoveryFiresOnReappearance();
    void latencyEmaConverges();
};

static QVector<QStringList> oneSensor(const QString &id) {
    QVector<QStringList> v;
    v.append(QStringList{id, "Temp " + id, "Lab"});
    return v;
}

void TestHealthMonitor::emitsAppearedOnFirstObservation() {
    HealthMonitor m;
    QSignalSpy spy(&m, &HealthMonitor::sensorAppeared);
    m.recordObservation(oneSensor("s1"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m.sensorCount(), 1);
    QCOMPARE(m.staleCount(), 0);
}

void TestHealthMonitor::marksSensorStaleAfterThreshold() {
    HealthMonitor m;
    m.setStaleAfterSecs(1);
    m.recordObservation(oneSensor("s1"));
    QCOMPARE(m.staleCount(), 0);

    QThread::sleep(2);
    QSignalSpy spy(&m, &HealthMonitor::sensorWentStale);
    m.evaluateStaleness();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m.staleCount(), 1);
}

void TestHealthMonitor::recoveryFiresOnReappearance() {
    HealthMonitor m;
    m.setStaleAfterSecs(1);
    m.recordObservation(oneSensor("s1"));
    QThread::sleep(2);
    m.evaluateStaleness();
    QCOMPARE(m.staleCount(), 1);

    QSignalSpy spy(&m, &HealthMonitor::sensorRecovered);
    m.recordObservation(oneSensor("s1"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m.staleCount(), 0);
}

void TestHealthMonitor::latencyEmaConverges() {
    HealthMonitor m;
    m.recordObservation(oneSensor("s1"));
    for (int i = 0; i < 100; ++i) m.recordLatency(100);
    const SensorHealth h = m.forId("s1");
    QVERIFY(qAbs(h.avgLatencyMs - 100.0) < 0.5);
}

QTEST_MAIN(TestHealthMonitor)
#include "test_health_monitor.moc"
