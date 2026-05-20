#include "mainwindow.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) 
{
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    statusLabel_ = new QLabel("connecting...", this);
    layout->addWidget(statusLabel_);

    table_ = new QTableWidget(0, 3, this);
    table_->setHorizontalHeaderLabels({"ID", "Name", "Location"});
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(table_);

    setCentralWidget(central);

    net_ = new QNetworkAccessManager(this);
    connect(net_, &QNetworkAccessManager::finished, this, &MainWindow::onSensorsReply);

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::refreshSensors);
    timer_->start(5000);
    refreshSensors();
}

void MainWindow::refreshSensors() {
    net_->get(QNetworkRequest(QUrl(backendUrl_ + "/sensors")));
}

void MainWindow::onSensorsReply(QNetworkReply* reply) 
{
    if (reply->error() != QNetworkReply::NoError) 
    {
        statusLabel_->setText("offline");
        statusLabel_->setStyleSheet("color: red;");
        reply->deleteLater();
        return;
    }

    statusLabel_->setText("connected");
    statusLabel_->setStyleSheet("color: green;");

    auto doc = QJsonDocument::fromJson(reply->readAll());
    auto arr = doc.array();
    table_->setRowCount(arr.size());

    for (int i = 0; i < arr.size(); ++i) 
    {
        auto obj = arr[i].toObject();
        table_->setItem(i, 0, new QTableWidgetItem(QString::number(obj["id"].toInt())));
        table_->setItem(i, 1, new QTableWidgetItem(obj["name"].toString()));
        table_->setItem(i, 2, new QTableWidgetItem(obj["location"].toString()));
    }
    
    reply->deleteLater();
}
// ---------------------------------------------------------------------
// AppContext wiring (added by feature batch)
// ---------------------------------------------------------------------
#include "app_context.h"
#include "http_client.h"
#include "sensor_health.h"
#include "notification_banner.h"
#include "health_dock.h"
#include "keyboard_shortcuts.h"
#include "shortcuts_dialog.h"
#include "theme_manager.h"

#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>

void MainWindow::setAppContext(sensorhub::AppContext *ctx) {
    m_appCtx = ctx;
    if (!m_appCtx) return;

    // Notification banner: parented to this window, shown above the
    // central widget via the status bar's permanent slot until the
    // central layout is refactored.
    if (!m_banner) {
        m_banner = new sensorhub::NotificationBanner(this);
        statusBar()->addPermanentWidget(m_banner, /*stretch*/ 1);
    }

    // Health dock
    if (!m_healthDock) {
        m_healthDock = new sensorhub::HealthDock(m_appCtx->health(), this);
        addDockWidget(Qt::RightDockWidgetArea, m_healthDock);
        m_healthDock->hide();
        installViewMenuEntries();
    }

    // Keyboard shortcuts
    if (!m_shortcuts) {
        m_shortcuts = new sensorhub::KeyboardShortcuts(this);
        m_shortcuts->bind(QStringLiteral("refresh"),
                          tr("Refresh now"),
                          QKeySequence(QStringLiteral("Ctrl+R")));
        m_shortcuts->bind(QStringLiteral("toggle_health"),
                          tr("Toggle Sensor Health dock"),
                          QKeySequence(QStringLiteral("Ctrl+H")));
        m_shortcuts->bind(QStringLiteral("show_shortcuts"),
                          tr("Show keyboard shortcuts"),
                          QKeySequence(QStringLiteral("F1")));
        m_shortcuts->bind(QStringLiteral("toggle_theme"),
                          tr("Toggle light/dark theme"),
                          QKeySequence(QStringLiteral("Ctrl+T")));
        connect(m_shortcuts, &sensorhub::KeyboardShortcuts::triggered,
                this, &MainWindow::onShortcutTriggered);
    }

    // HealthMonitor signal hook-ups for the banner
    auto *health = m_appCtx->health();
    connect(health, &sensorhub::HealthMonitor::sensorWentStale,
            this, &MainWindow::onSensorWentStale);
    connect(health, &sensorhub::HealthMonitor::sensorRecovered,
            this, &MainWindow::onSensorRecovered);
}

void MainWindow::onShortcutTriggered(const QString &id) {
    if (!m_appCtx) return;
    if (id == QLatin1String("refresh")) {
        // No-op stub: existing auto-refresh timer in MainWindow drives
        // periodic updates; manual refresh wiring is the next batch.
        statusBar()->showMessage(tr("Refresh requested"), 1500);
    } else if (id == QLatin1String("toggle_health") && m_healthDock) {
        m_healthDock->setVisible(!m_healthDock->isVisible());
    } else if (id == QLatin1String("show_shortcuts")) {
        sensorhub::ShortcutsDialog dlg(m_shortcuts, this);
        dlg.exec();
    } else if (id == QLatin1String("toggle_theme")) {
        auto *theme = m_appCtx->theme();
        using T = sensorhub::ThemeManager;
        theme->apply(theme->current() == T::Dark ? T::Light : T::Dark);
        theme->persist();
    }
}

void MainWindow::onSensorWentStale(const QString &id, const QString &name) {
    if (m_banner) {
        m_banner->post(tr("Sensor %1 (%2) is stale").arg(name, id),
                       sensorhub::NotificationBanner::Warning);
    }
}

void MainWindow::onSensorRecovered(const QString &id, const QString &name) {
    if (m_banner) {
        m_banner->post(tr("Sensor %1 (%2) recovered").arg(name, id),
                       sensorhub::NotificationBanner::Info);
    }
}


void MainWindow::installViewMenuEntries() {
    if (!m_healthDock || !menuBar()) return;
    QMenu *viewMenu = nullptr;
    for (QAction *a : menuBar()->actions()) {
        if (a->menu() && a->text().contains(tr("View"), Qt::CaseInsensitive)) {
            viewMenu = a->menu();
            break;
        }
    }
    if (!viewMenu) {
        viewMenu = menuBar()->addMenu(tr("&View"));
    }
    viewMenu->addAction(m_healthDock->toggleViewAction());
}
