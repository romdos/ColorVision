cmake_minimum_required(VERSION 3.10)
project(ColorVision)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS} include)


set(SOURCE_FILES
        main.cpp
        src/Strip.cpp
        src/Bunchrel.cpp
        src/BunchGray.cpp
        src/ColorIntervalSelect.cpp
        src/ColorSection.cpp
        src/ImageProcess.cpp
        src/RoadGrSky.cpp
        src/SignalZones.cpp
        src/Segment.cpp
        src/Geometry.cpp
        src/MarkingDetector.cpp)

add_executable(ColorVision2 ${SOURCE_FILES})

target_link_libraries(ColorVision2 ${OpenCV_LIBS})

