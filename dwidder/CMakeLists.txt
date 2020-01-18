project(dwidder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED Core Widgets )


if(UNIX)
add_definitions(
    -Wfatal-errors
    -Og
    )
endif()

qt5_add_resources(RSCS
#    resources/resources.qrc
)

set(DWIDDER_SOURCES
    main.cpp
    EventProxy.cpp
    MainWindow.cpp
    DwidderApp.cpp
    dwidder_utils.cpp
    channels/announcements/announcements.cpp
    channels/announcements/announcements_channel.cpp
    channels/calendar/calendar.cpp
    channels/calendar/calendar_channel.cpp
    channels/weather/weather_channel.cpp
    channels/mandates/mandates_channel.cpp
    channels/crime/crime_channel.cpp
    channels/activities/activities_channel.cpp
    channels/incidents/incidents_channel.cpp
    channels/buildings/building_channel.cpp
    channels/items/items_channel.cpp
    ${RSCS}
)

qt5_wrap_cpp(DWIDDER_MOC_SOURCES
    EventProxy.h
    MainWindow.h
)


#qt5_wrap_ui(DWIDDER_UI_SOURCES
#    MainWindow.ui
#)


include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
#    ${QtQml_INCLUDE_DIRS}
#    ${QtQuick_INCLUDE_DIRS}
)

add_definitions(
    ${Qt5Widgets_DEFINITIONS}
#    ${QtQml_DEFINITIONS}
#    ${${Qt5Quick_DEFINITIONS}}
    ${${QtWidgets_DEFINITIONS}}
)



DFHACK_PLUGIN(dwidder
    ${DWIDDER_SOURCES} ${DWIDDER_MOC_SOURCES} ${DWIDDER_UI_SOURCES} ${RSCS}
    LINK_LIBRARIES Qt5::Core Qt5::Widgets)
target_include_directories(dwidder PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
