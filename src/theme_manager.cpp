#include "theme_manager.h"

#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QSettings>
#include <QStyleFactory>

namespace sensorhub {

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent), m_current(System) {}

QString ThemeManager::themeName(Theme t) {
    switch (t) {
        case Light:  return QStringLiteral("light");
        case Dark:   return QStringLiteral("dark");
        case System: return QStringLiteral("system");
    }
    return QStringLiteral("system");
}

void ThemeManager::apply(Theme t) {
    m_current = t;
    switch (t) {
        case Light:  applyLight();  break;
        case Dark:   applyDark();   break;
        case System: applySystem(); break;
    }
    emit themeChanged(t);
}

void ThemeManager::applyLight() {
    if (auto *app = qobject_cast<QApplication *>(QApplication::instance())) {
        app->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
        QPalette p;
        p.setColor(QPalette::Window,        QColor(245, 245, 247));
        p.setColor(QPalette::WindowText,    Qt::black);
        p.setColor(QPalette::Base,          Qt::white);
        p.setColor(QPalette::AlternateBase, QColor(238, 238, 240));
        p.setColor(QPalette::Text,          Qt::black);
        p.setColor(QPalette::Button,        QColor(238, 238, 240));
        p.setColor(QPalette::ButtonText,    Qt::black);
        p.setColor(QPalette::Highlight,     QColor(38, 110, 190));
        p.setColor(QPalette::HighlightedText, Qt::white);
        app->setPalette(p);
        app->setStyleSheet(QString());
    }
}

void ThemeManager::applyDark() {
    if (auto *app = qobject_cast<QApplication *>(QApplication::instance())) {
        app->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
        QPalette p;
        p.setColor(QPalette::Window,         QColor(45, 45, 48));
        p.setColor(QPalette::WindowText,     QColor(220, 220, 220));
        p.setColor(QPalette::Base,           QColor(30, 30, 32));
        p.setColor(QPalette::AlternateBase,  QColor(40, 40, 42));
        p.setColor(QPalette::Text,           QColor(220, 220, 220));
        p.setColor(QPalette::Button,         QColor(55, 55, 58));
        p.setColor(QPalette::ButtonText,     QColor(220, 220, 220));
        p.setColor(QPalette::Highlight,      QColor(38, 110, 190));
        p.setColor(QPalette::HighlightedText, Qt::white);
        p.setColor(QPalette::ToolTipBase,    QColor(50, 50, 55));
        p.setColor(QPalette::ToolTipText,    QColor(220, 220, 220));
        p.setColor(QPalette::Disabled, QPalette::Text,       QColor(120, 120, 120));
        p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));
        app->setPalette(p);
        app->setStyleSheet(QStringLiteral(
            "QToolTip { color: #dddddd; background-color: #323237; border: 1px solid #555; }"
            "QTableWidget { gridline-color: #444; }"
        ));
    }
}

void ThemeManager::applySystem() {
    if (auto *app = qobject_cast<QApplication *>(QApplication::instance())) {
        app->setStyle(QStyleFactory::create(app->style()->objectName()));
        app->setPalette(app->style()->standardPalette());
        app->setStyleSheet(QString());
    }
}

void ThemeManager::loadPersisted() {
    QSettings s;
    const QString name = s.value(QStringLiteral("ui/theme"),
                                 QStringLiteral("system")).toString();
    if (name == QLatin1String("light"))      apply(Light);
    else if (name == QLatin1String("dark"))  apply(Dark);
    else                                     apply(System);
}

void ThemeManager::persist() const {
    QSettings s;
    s.setValue(QStringLiteral("ui/theme"), themeName(m_current));
}

} // namespace sensorhub
