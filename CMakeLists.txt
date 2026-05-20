cmake_minimum_required(VERSION 3.14)
project(sensorhub-qt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
    src/sensor_detail.cpp
    src/app_context.cpp
    src/http_client.cpp
    src/sensor_health.cpp
    src/notification_banner.cpp
    src/health_dock.cpp
    src/theme_manager.cpp
    src/keyboard_shortcuts.cpp
    src/shortcuts_dialog.cpp
    inc/app_context.h
    inc/http_client.h
    inc/sensor_health.h
    inc/notification_banner.h
    inc/health_dock.h
    inc/theme_manager.h
    inc/keyboard_shortcuts.h
    inc/shortcuts_dialog.h
    inc/mainwindow.h
    inc/readings_table.h
    inc/settings_dialog.h
    inc/about_dialog.h
    inc/status_bar.h
    inc/log_viewer.h
    inc/chart_widget.h
    inc/sensor_detail.h
)

target_include_directories(sensorhub-monitor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(sensorhub-monitor PRIVATE
    Qt5::Widgets
    Qt5::Network
)
