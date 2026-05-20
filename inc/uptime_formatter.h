#ifndef SENSORHUB_UPTIME_FORMATTER_H
#define SENSORHUB_UPTIME_FORMATTER_H

#include <QString>

namespace sensorhub {

// Format a non-negative duration in seconds as "Xd Yh Zm" / "Yh Zm" /
// "Zm Ws" / "Ws". Returns "--" for zero or negative input.
QString formatUptime(qint64 secs);

} // namespace sensorhub

#endif // SENSORHUB_UPTIME_FORMATTER_H
