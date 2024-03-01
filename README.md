# Сборка Qt 5.15.12 Cadabra Edition

0. Создать необходимые директории:
```bash
mkdir -p <Qt5.15.2.CE-source-path>
mkdir -p <Qt5.15.2.CE-install-path>
```

1. Склонировать репозиторий с Qt5.15.12 Cadabra Edition:
```bash
git clone https://github.com/cadabra-systems/Qt5.15.12.CE.git <Qt5.15.2.CE-source-path>
```

## Сборка для Android на Linux-based OS

## Сборка для Android на macOS
2a. Установить переменные окружения:
```bash
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk
export PATH=$JAVA_HOME/bin:$PATH
```

2b. Запустить скрипт конфигурации:
```bash
<Qt5.15.2.CE-source-path>/configure -xplatform android-clang \
-I<OpenSSL1.1.1m-source-path>/include \
-L<OpenSSL1.1.1m-library-path> \
-prefix <Qt5.15.2.CE-install-path> \
-disable-rpath \
-android-ndk <AndroidSDK-root-path>/ndk/22.1.7171670 \
-android-sdk <AndroidSDK-root-path> \
-no-warnings-are-errors \
-ssl \
-sql-sqlite \
-opensource \
-confirm-license \
-debug -separate-debug-info \
-nomake tests \
-nomake examples
```

## Сборка для iOS на macOS
2. Запустить скрипт конфигурации:
```bash
<Qt5.15.2.CE-source-path>/configure -xplatform macx-ios-clang \
-prefix <Qt5.15.2.CE-install-path> \
-sql-sqlite \
-opensource \
-confirm-license \
-debug-and-release \
-nomake tests \
-nomake examples
```
    
3. Запустить сборку:
Для удобства запуска многопоточной сборки определить алиас: `alias nproc="sysctl -n hw.logicalcpu"`
```bash
make --jobs=$(nproc)
sudo make install
```