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
    src/sensor_detail.cpp
)

target_include_directories(sensorhub-monitor PRIVATE src)
target_link_libraries(sensorhub-monitor Qt5::Widgets Qt5::Network)