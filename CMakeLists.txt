cmake_minimum_required(VERSION 3.14)
project(sensorhub-qt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Network)

add_executable(sensorhub-monitor
    src/main.cpp
    src/mainwindow.cpp
    src/readings_table.cpp
    src/settings_dialog.cpp
    src/about_dialog.cpp
    src/status_bar.cpp
    src/log_viewer.cpp
    src/chart_widget.cpp
    src/status_bar.cpp
    src/log_viewer.cpp
    src/chart_widget.cpp
    src/log_viewer.cpp
    src/chart_widget.cpp
    src/sensor_detail.cpp

    src/http_client.cpp
    inc/http_client.h
    src/sensor_health.cpp
    inc/sensor_health.h
    src/notification_banner.cpp
    inc/notification_banner.h
    src/health_dock.cpp
    inc/health_dock.h
    src/theme_manager.cpp
    inc/theme_manager.h
    src/keyboard_shortcuts.cpp
    inc/keyboard_shortcuts.h
    src/shortcuts_dialog.cpp
    inc/shortcuts_dialog.h
)

target_include_directories(sensorhub-monitor PRIVATE src)
target_link_libraries(sensorhub-monitor Qt5::Widgets Qt5::Network)