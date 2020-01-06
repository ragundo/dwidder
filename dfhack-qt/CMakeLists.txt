find_package(Qt5 REQUIRED Core Widgets)

add_library(dfhack-qt-common INTERFACE IMPORTED GLOBAL)
target_include_directories(dfhack-qt-common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(qapplication)
