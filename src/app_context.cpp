#include "app_context.h"
#include "http_client.h"
#include "sensor_health.h"
#include "theme_manager.h"

namespace sensorhub {

AppContext::AppContext(QObject *parent)
    : QObject(parent),
      m_http(new HttpClient(this)),
      m_health(new HealthMonitor(this)),
      m_theme(new ThemeManager(this)) {
    // Pipe every successful HTTP latency sample into the health
    // monitor so its EMA reflects real network behaviour.
    connect(m_http, &HttpClient::latencySample,
            this, [this](int, qint64 ms) {
        m_health->recordLatency(ms);
    });
}

AppContext::~AppContext() = default;

} // namespace sensorhub
