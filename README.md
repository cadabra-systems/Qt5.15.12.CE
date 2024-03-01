# Сборка для Android на Linux-based OS

# Сборка для Android на macOS
...
# Сборка для iOS на macOS

1. Создать директорию для сборки:  `sudo mkdir -p /build/qt/5.15.12.CE`

2. Запустить скрипт конфигурации:
    
    ```bash
    <путь к исходникам Qt>/configure \
    -xplatform macx-ios-clang \
    -prefix /build/qt/5.15.12.CE \
    -sql-sqlite \
    -opensource \
    -confirm-license \
    -debug-and-release \
    -nomake tests \
    -nomake examples
    ```
    
3. Запустить сборку:
    
    Для удобства сборки сделаем алиас `alias nproc="sysctl -n hw.logicalcpu"`

    ```bash
    make --jobs=$(nproc)
    sudo make install
    ```