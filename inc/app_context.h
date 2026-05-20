#ifndef SENSORHUB_APP_CONTEXT_H
#define SENSORHUB_APP_CONTEXT_H

#include <QObject>

namespace sensorhub {

class HttpClient;
class HealthMonitor;
class ThemeManager;

// Aggregates the long-lived services used across the UI. Owned by
// main() and passed by pointer to widgets that need them so we do
// not rely on globals.
class AppContext : public QObject {
    Q_OBJECT
public:
    explicit AppContext(QObject *parent = nullptr);
    ~AppContext() override;

    HttpClient    *http()   const { return m_http; }
    HealthMonitor *health() const { return m_health; }
    ThemeManager  *theme()  const { return m_theme; }

private:
    HttpClient    *m_http;
    HealthMonitor *m_health;
    ThemeManager  *m_theme;
};

} // namespace sensorhub

#endif // SENSORHUB_APP_CONTEXT_H
