#include "uptime_formatter.h"

namespace sensorhub {

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

} // namespace sensorhub
