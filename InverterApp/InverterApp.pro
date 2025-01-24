QT       += core gui serialport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    serialparamslayout.cpp

HEADERS += \
    mainwindow.h \
    serialparamslayout.h

FORMS += \
    mainwindow.ui \
    serialparamslayout.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../InverterApp.pro/AndroidManifest.xml \
    ../InverterApp.pro/build.gradle \
    ../InverterApp.pro/gradle.properties \
    ../InverterApp.pro/gradle/wrapper/gradle-wrapper.jar \
    ../InverterApp.pro/gradle/wrapper/gradle-wrapper.properties \
    ../InverterApp.pro/gradlew \
    ../InverterApp.pro/gradlew.bat \
    ../InverterApp.pro/res/values/libs.xml \
    ../InverterApp.pro/res/xml/qtprovider_paths.xml \
    ../android/AndroidManifest.xml \
    ../android/build.gradle \
    ../android/res/values/libs.xml \
    ../android/res/xml/qtprovider_paths.xml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/build.gradle \
    android/res/values/libs.xml \
    android/res/values/libs.xml \
    android/res/xml/qtprovider_paths.xml \
    android/res/xml/qtprovider_paths.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/../InverterApp.pro
}

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/../InverterApp/android
}
