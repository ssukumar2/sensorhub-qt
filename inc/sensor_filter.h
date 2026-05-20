#ifndef SENSORHUB_SENSOR_FILTER_H
#define SENSORHUB_SENSOR_FILTER_H

#include <QSortFilterProxyModel>
#include <QString>

namespace sensorhub {

// Case-insensitive substring filter that searches every column of
// the source model. Connect a QLineEdit::textChanged to setQuery().
class SensorFilter : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit SensorFilter(QObject *parent = nullptr);

public slots:
    void setQuery(const QString &needle);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;

private:
    QString m_needle;
};

} // namespace sensorhub

#endif // SENSORHUB_SENSOR_FILTER_H
