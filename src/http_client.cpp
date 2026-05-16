#include "http_client.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDateTime>
#include <QJsonParseError>
#include <QVariant>

namespace sensorhub {

HttpClient::HttpClient(QObject *parent)
    : QObject(parent),
      m_nam(new QNetworkAccessManager(this)),
      m_baseUrl(QStringLiteral("http://localhost:8000")),
      m_timeoutMs(4000),
      m_nextId(1) {
    connect(m_nam, &QNetworkAccessManager::finished,
            this, &HttpClient::onFinished);
}

HttpClient::~HttpClient() = default;

void HttpClient::setBaseUrl(const QString &baseUrl) {
    m_baseUrl = baseUrl;
    while (m_baseUrl.endsWith(QLatin1Char('/'))) {
        m_baseUrl.chop(1);
    }
}

void HttpClient::setTimeoutMs(int ms) {
    if (ms > 0) {
        m_timeoutMs = ms;
    }
}

QUrl HttpClient::buildUrl(const QString &path) const {
    QString p = path;
    if (!p.startsWith(QLatin1Char('/'))) {
        p.prepend(QLatin1Char('/'));
    }
    return QUrl(m_baseUrl + p);
}

int HttpClient::getJson(const QString &path) {
    const int id = m_nextId++;
    QNetworkRequest req(buildUrl(path));
    req.setRawHeader("Accept", "application/json");
    req.setAttribute(QNetworkRequest::User, id);
    req.setAttribute(QNetworkRequest::Attribute(QNetworkRequest::User + 1), true);
    req.setAttribute(QNetworkRequest::Attribute(QNetworkRequest::User + 2),
                     QDateTime::currentMSecsSinceEpoch());

    QNetworkReply *reply = m_nam->get(req);

    QTimer *t = new QTimer(reply);
    t->setSingleShot(true);
    connect(t, &QTimer::timeout, reply, [reply]() {
        if (reply->isRunning()) {
            reply->abort();
        }
    });
    t->start(m_timeoutMs);
    return id;
}

int HttpClient::getRaw(const QString &path) {
    const int id = m_nextId++;
    QNetworkRequest req(buildUrl(path));
    req.setAttribute(QNetworkRequest::User, id);
    req.setAttribute(QNetworkRequest::Attribute(QNetworkRequest::User + 1), false);
    req.setAttribute(QNetworkRequest::Attribute(QNetworkRequest::User + 2),
                     QDateTime::currentMSecsSinceEpoch());

    QNetworkReply *reply = m_nam->get(req);

    QTimer *t = new QTimer(reply);
    t->setSingleShot(true);
    connect(t, &QTimer::timeout, reply, [reply]() {
        if (reply->isRunning()) {
            reply->abort();
        }
    });
    t->start(m_timeoutMs);
    return id;
}

void HttpClient::onFinished(QNetworkReply *reply) {
    reply->deleteLater();

    const int id = reply->request().attribute(QNetworkRequest::User).toInt();
    const bool wantJson = reply->request()
                              .attribute(QNetworkRequest::Attribute(QNetworkRequest::User + 1))
                              .toBool();
    const qint64 started = reply->request()
                               .attribute(QNetworkRequest::Attribute(QNetworkRequest::User + 2))
                               .toLongLong();
    const qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - started;
    emit latencySample(id, elapsed);

    if (reply->error() != QNetworkReply::NoError) {
        emit requestFailed(id, reply->errorString());
        return;
    }

    const QByteArray body = reply->readAll();
    if (!wantJson) {
        emit rawReceived(id, body);
        return;
    }

    QJsonParseError perr{};
    QJsonDocument doc = QJsonDocument::fromJson(body, &perr);
    if (perr.error != QJsonParseError::NoError) {
        emit requestFailed(id, QStringLiteral("JSON parse error: %1").arg(perr.errorString()));
        return;
    }
    emit jsonReceived(id, doc);
}

} // namespace sensorhub
