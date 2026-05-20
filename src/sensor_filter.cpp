#include "sensor_filter.h"

#include <QAbstractItemModel>

namespace sensorhub {

SensorFilter::SensorFilter(QObject *parent)
    : QSortFilterProxyModel(parent) {
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void SensorFilter::setQuery(const QString &needle) {
    m_needle = needle.trimmed();
    invalidateFilter();
}

bool SensorFilter::filterAcceptsRow(int sourceRow,
                                    const QModelIndex &sourceParent) const {
    if (m_needle.isEmpty()) return true;
    QAbstractItemModel *src = sourceModel();
    if (!src) return true;
    const int cols = src->columnCount(sourceParent);
    for (int c = 0; c < cols; ++c) {
        const QModelIndex idx = src->index(sourceRow, c, sourceParent);
        if (idx.data(Qt::DisplayRole).toString()
                .contains(m_needle, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

} // namespace sensorhub
