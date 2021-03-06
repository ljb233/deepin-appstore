cmake_minimum_required(VERSION 3.2)
project(Shell)
set(SHELL_VERSION 4.0.9)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

#add_definitions(-D DEBUG_LASTORE=1)
#add_definitions(-D DEBUG_LOCAL_REQUEST=1)

# deepin-wm and mutter support the _NET_WM_OPAQUE_REGION
# However this optimization causes uncomposited area to be black during window resizing
# keep it disabled for now
add_definitions(-D USE_OPAQUE_REGION_OPTIMIZATION=0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt5Network REQUIRED)

pkg_search_module(DTKWIDGET REQUIRED dtkwidget)
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(X11 REQUIRED x11)
pkg_search_module(X11Ext REQUIRED xext)
pkg_search_module(GIO REQUIRED gio-unix-2.0)

include_directories(AFTER ${DTKWIDGET_INCLUDE_DIRS})
include_directories(AFTER ${GLIB_INCLUDE_DIRS})
include_directories(AFTER ${X11_INCLUDE_DIRS})
include_directories(AFTER ${X11Ext_INCLUDE_DIRS})
include_directories(AFTER ${GIO_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp
                 MainWindow.cpp MainWindow.h
                 Shell.cpp Shell.h
                 Bridge.cpp Bridge.h
                 FilterMouseMove.cpp FilterMouseMove.h
                 configure.h configure.cpp
                 AboutWindow.cpp AboutWindow.h
                 lastore-daemon.h
                 DBusInterface.cpp DBusInterface.h)

set(TOOLTIP_FILES ToolTip.cpp)
set(DBUSMENU_FILES dbusmenu.cpp dbusmenu.h dbusmenumanager.cpp dbusmenumanager.h)
set(DBUS_BACKEND_FILES LAStoreBridge.cpp LAStoreBridge.h)

set(LINK_LIBS
        Qt5::Widgets
        Qt5::X11Extras
        Qt5::DBus
        Qt5::Gui
        Qt5::Network
        ${GLIB_LIBRARIES}
        ${DTKWIDGET_LIBRARIES}
        ${X11_LIBRARIES}
        ${X11Ext_LIBRARIES}
        ${GIO_LIBRARIES}
)

add_definitions(-DSHELL_VERSION="${SHELL_VERSION}")

# Link against either QtWebKit or QtWebEngine
#find_package(Qt5WebEngineWidgets)
# track qt web engine missing features: https://trello.com/b/5G9c1rkb/qtwebengine
if(Qt5WebEngineWidgets_FOUND)
    find_package(Qt5WebChannel REQUIRED)
    set(WEB_WIDGET_FILES WebEngine/WebView.cpp WebEngine/WebView.h WebEngine/WebPage.cpp WebEngine/WebPage.h)
    add_definitions(-D BUILD_WITH_WEBENGINE=1)
    add_executable(deepin-appstore ${SOURCE_FILES} ${TOOLTIP_FILES} ${DBUSMENU_FILES} ${DBUS_BACKEND_FILES} ${WEB_WIDGET_FILES}
                   res.qrc)
    target_link_libraries(deepin-appstore ${LINK_LIBS} Qt5::WebEngineWidgets Qt5::WebChannel)
else()
    set(WEB_WIDGET_FILES WebKit/WebView.cpp WebKit/WebView.h
                         WebKit/WebPage.cpp WebKit/WebPage.h
                         WebKit/NetworkAccessManager.cpp WebKit/NetworkAccessManager.h
                         WebKit/CookieJar.cpp WebKit/CookieJar.h
                         WebKit/LocalFileSystemReply.cpp WebKit/LocalFileSystemReply.h)

    find_package(Qt5WebKit REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
    add_definitions(-DBUILD_WITH_WEBKIT=1)
    add_executable(deepin-appstore ${SOURCE_FILES} ${TOOLTIP_FILES} ${DBUSMENU_FILES} ${DBUS_BACKEND_FILES} ${WEB_WIDGET_FILES}
                   res.qrc)

    target_link_libraries(deepin-appstore ${LINK_LIBS} Qt5::WebKitWidgets)
endif()

install(TARGETS deepin-appstore
        DESTINATION "/usr/bin")
install(FILES support/deepin-appstore.desktop
        DESTINATION "/usr/share/applications")
install(DIRECTORY support/icons/
        DESTINATION "/usr/share/icons/")
install(DIRECTORY manual/
        DESTINATION "/usr/share/dman/deepin-appstore")
