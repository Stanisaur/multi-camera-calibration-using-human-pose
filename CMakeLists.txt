cmake_minimum_required(VERSION 3.16)

project(MultiCameraCalibration VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)

find_package( OpenCV REQUIRED )
find_package( onnxruntime REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MultiCameraCalibration
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        images.qrc
        cameracell.h cameracell.cpp cameracell.ui
        PoseEstimation/characterset_convert.h PoseEstimation/rtmdet_onnxruntime.h PoseEstimation/rtmpose_onnxruntime.h PoseEstimation/rtmpose_tracker_onnxruntime.h PoseEstimation/rtmpose_utils.h
        utils.cpp
        utils.h
        PoseEstimation/rtmdet_onnxruntime.cpp PoseEstimation/rtmpose_onnxruntime.cpp PoseEstimation/rtmpose_tracker_onnxruntime.cpp PoseEstimation/rtmpose_utils.h
        PoseEstimation/models/rtmdet-389d3a.onnx PoseEstimation/models/rtmpose-s_simcc-body7_pt-body7_420e.onnx


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MultiCameraCalibration APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MultiCameraCalibration SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MultiCameraCalibration
            ${PROJECT_SOURCES}
            # images.qrc
        )
    endif()
endif()

target_link_libraries(MultiCameraCalibration PRIVATE Qt6::Multimedia)
target_link_libraries(MultiCameraCalibration PUBLIC Qt::MultimediaWidgets)

target_link_libraries(MultiCameraCalibration PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MultiCameraCalibration PUBLIC onnxruntime::onnxruntime)
target_link_libraries(MultiCameraCalibration PUBLIC ${OpenCV_LIBS})


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MultiCameraCalibration)
endif()
set_target_properties(MultiCameraCalibration PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MultiCameraCalibration
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MultiCameraCalibration)
endif()
