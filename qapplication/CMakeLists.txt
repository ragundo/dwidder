project(qapplication)

set(QAPPLICATION_SOURCES
	main.cpp
	Application.cpp
    Log.cpp
    LogWindow.cpp
)

qt5_wrap_cpp(QAPPLICATION_MOC_SOURCES
	Application.h
    Log.h
    LogWindow.h
)

qt5_wrap_ui(QAPPLICATION_UI_SOURCES
    LogWindow.ui
)


include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${QtQml_INCLUDE_DIRS}
    ${QtQuick_INCLUDE_DIRS}
)

add_definitions(
    ${Qt5Widgets_DEFINITIONS}
    ${QtQml_DEFINITIONS}
    ${${Qt5Quick_DEFINITIONS}}
    ${${QtWidgets_DEFINITIONS}}
)

DFHACK_PLUGIN(qapplication
    ${QAPPLICATION_SOURCES} ${QAPPLICATION_MOC_SOURCES} ${QAPPLICATION_UI_SOURCES}
    LINK_LIBRARIES Qt5::Core Qt5::Widgets dfhack-tinythread )
target_include_directories(qapplication PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
