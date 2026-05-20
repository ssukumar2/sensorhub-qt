#include "health_dock.h"
#include "sensor_health.h"
#include "uptime_formatter.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QDateTime>
#include <QBrush>
#include <QColor>

namespace sensorhub {

namespace {
constexpr int kColId=0, kColName=1, kColLocation=2, kColLastSeen=3,
              kColUptime=4, kColLatency=5, kColStatus=6;

QString formatUptime(qint64 secs) {
    if (secs <= 0) return QStringLiteral("--");
    const qint64 d = secs / 86400;
    const qint64 h = (secs % 86400) / 3600;
    const qint64 m = (secs % 3600) / 60;
    const qint64 s = secs % 60;
    if (d > 0) return QStringLiteral("%1d %2h %3m").arg(d).arg(h).arg(m);
    if (h > 0) return QStringLiteral("%1h %2m").arg(h).arg(m);
    if (m > 0) return QStringLiteral("%1m %2s").arg(m).arg(s);
    return QStringLiteral("%1s").arg(s);
}
} // namespace

HealthDock::HealthDock(HealthMonitor *monitor, QWidget *parent)
    : QDockWidget(tr("Sensor Health"), parent),
      m_monitor(monitor), m_table(nullptr),
      m_summary(nullptr), m_resetBtn(nullptr) {
    setObjectName(QStringLiteral("HealthDock"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    buildUi();
    if (m_monitor) {
        connect(m_monitor, &HealthMonitor::snapshotChanged, this, &HealthDock::refresh);
    }
    refresh();
}

void HealthDock::buildUi() {
    QWidget *root = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(root);
    lay->setContentsMargins(6, 6, 6, 6);
    lay->setSpacing(4);

    m_summary = new QLabel(root);
    m_summary->setTextInteractionFlags(Qt::TextSelectableByMouse);
    lay->addWidget(m_summary);

    m_table = new QTableWidget(root);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels(QStringList{
        tr("ID"), tr("Name"), tr("Location"),
        tr("Last Seen"), tr("Uptime"), tr("Avg Latency"), tr("Status")
    });
    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->setSortingEnabled(true);
    m_table->horizontalHeader()->setStretchLastSection(true);
    lay->addWidget(m_table, 1);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addStretch(1);
    m_resetBtn = new QPushButton(tr("Reset counters"), root);
    btnRow->addWidget(m_resetBtn);
    lay->addLayout(btnRow);

    setWidget(root);
    connect(m_resetBtn, &QPushButton::clicked, this, &HealthDock::onResetClicked);
}

void HealthDock::onResetClicked() { if (m_monitor) m_monitor->reset(); }

void HealthDock::onStaleThresholdChanged(int secs) {
    if (m_monitor) { m_monitor->setStaleAfterSecs(secs); m_monitor->evaluateStaleness(); refresh(); }
}

void HealthDock::refresh() {
    if (!m_monitor) return;
    const QVector<SensorHealth> rows = m_monitor->snapshot();
    m_table->setSortingEnabled(false);
    m_table->setRowCount(rows.size());

    int row = 0;
    for (const SensorHealth &h : rows) {
        auto setCell = [&](int col, const QString &text, const QBrush &fg = QBrush()) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            if (fg.style() != Qt::NoBrush) item->setForeground(fg);
            m_table->setItem(row, col, item);
        };
        setCell(kColId, h.id);
        setCell(kColName, h.name);
        setCell(kColLocation, h.location);
        setCell(kColLastSeen,
                h.lastSeen.isValid()
                    ? h.lastSeen.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"))
                    : QStringLiteral("--"));
        setCell(kColUptime, formatUptime(h.uptimeSecs()));
        setCell(kColLatency,
                h.avgLatencyMs > 0.0
                    ? QStringLiteral("%1 ms").arg(h.avgLatencyMs, 0, 'f', 1)
                    : QStringLiteral("--"));
        QBrush statusFg = h.stale ? QBrush(QColor(176, 0, 32))
                                  : QBrush(QColor(0, 120, 64));
        setCell(kColStatus, h.stale ? tr("STALE") : tr("OK"), statusFg);
        ++row;
    }
    m_table->setSortingEnabled(true);
    m_table->resizeColumnsToContents();

    const int total = m_monitor->sensorCount();
    const int stale = m_monitor->staleCount();
    m_summary->setText(tr("Tracked: %1   |   Stale: %2   |   Healthy: %3")
                           .arg(total).arg(stale).arg(total - stale));
}

} // namespace sensorhub
