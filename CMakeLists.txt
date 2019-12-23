find_package(Qt5 REQUIRED Widgets Qml Quick QuickWidgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(dfhack-qt)
add_subdirectory(dwidder)
