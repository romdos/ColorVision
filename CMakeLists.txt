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
        src/MarkingDetector.cpp
        include/ColorIntervalSelect.h
        include/ColorSection.h
        include/ImageProcess.h
        include/Segment.h
        include/MarkingDetector.h
        include/BunchGray.h
        include/Geometry.h
        include/Strip.h
        )

add_executable(ColorVision2 ${SOURCE_FILES})

target_link_libraries(ColorVision2 ${OpenCV_LIBS})

