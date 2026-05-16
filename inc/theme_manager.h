#ifndef SENSORHUB_THEME_MANAGER_H
#define SENSORHUB_THEME_MANAGER_H

#include <QObject>
#include <QString>

namespace sensorhub {

class ThemeManager : public QObject {
    Q_OBJECT
public:
    enum Theme { System, Light, Dark };
    Q_ENUM(Theme)

    explicit ThemeManager(QObject *parent = nullptr);

    Theme  current() const { return m_current; }
    void   apply(Theme t);
    void   loadPersisted();
    void   persist() const;

    static QString themeName(Theme t);

signals:
    void themeChanged(Theme t);

private:
    void applyLight();
    void applyDark();
    void applySystem();

    Theme m_current;
};

} // namespace sensorhub

#endif // SENSORHUB_THEME_MANAGER_H
