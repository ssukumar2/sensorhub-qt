#ifndef SENSORHUB_HTTP_CLIENT_H
#define SENSORHUB_HTTP_CLIENT_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QByteArray>
#include <QJsonDocument>

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

namespace sensorhub {

class HttpClient : public QObject {
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient() override;

    void setBaseUrl(const QString &baseUrl);
    QString baseUrl() const { return m_baseUrl; }

    void setTimeoutMs(int ms);
    int  timeoutMs() const { return m_timeoutMs; }

    int getJson(const QString &path);
    int getRaw(const QString &path);

signals:
    void jsonReceived(int requestId, const QJsonDocument &doc);
    void rawReceived(int requestId, const QByteArray &data);
    void requestFailed(int requestId, const QString &reason);
    void latencySample(int requestId, qint64 elapsedMs);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_nam;
    QString                m_baseUrl;
    int                    m_timeoutMs;
    int                    m_nextId;

    QUrl buildUrl(const QString &path) const;
};

} // namespace sensorhub

#endif // SENSORHUB_HTTP_CLIENT_H
