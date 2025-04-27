cmake_minimum_required(VERSION 3.16)

project(CS499 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        source/inventorywindow.cpp
        header/inventorywindow.h
        ui/inventorywindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CS499
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        header/additemdialog.h source/additemdialog.cpp ui/additemdialog.ui
        source/Lumber.cpp
        header/cutlistdialog.h source/cutlistdialog.cpp ui/cutlistdialog.ui

        header/modifyitemdialog.h source/modifyitemdialog.cpp ui/modifyitemdialog.ui
        header/photodialog.h source/photodialog.cpp ui/photodialog.ui
        header/soldinventorydialog.h source/soldinventorydialog.cpp ui/soldinventorydialog.ui
        header/soldinventoryviewer.h source/soldinventoryviewer.cpp ui/soldinventoryviewer.ui
        header/pricedialog.h source/pricedialog.cpp ui/pricedialog.ui
        source/CutList.cpp
        header/help.h source/help.cpp ui/help.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CS499 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CS499 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CS499
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CS499 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CS499)
endif()
set_target_properties(CS499 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CS499
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CS499)
endif()
