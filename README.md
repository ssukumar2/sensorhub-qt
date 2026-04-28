# sensorhub-qt

Qt5 desktop monitoring client for the sensorhub IoT backend. Shows connected sensors in a table with auto-refresh, and indicates backend connection status with a live indicator.

## Features

- Sensor list table with ID, name, and location
- Auto-refresh every 5 seconds
- Connection status indicator (green/red)
- Lightweight Qt Widgets interface

## Building

Requires Qt5 development packages and CMake.

    sudo apt install qtbase5-dev qt5-default
    cmake -B build
    cmake --build build
    ./build/sensorhub-monitor

## Stack

- C++17 with Qt5 Widgets and Qt5 Network
- CMake with AUTOMOC
- Connects to sensorhub backend on localhost:8000

## License

MIT