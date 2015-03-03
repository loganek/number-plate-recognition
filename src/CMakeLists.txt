include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(algorithms)

add_executable(recognition main.cpp MainWindow.cpp AlgorithmManager.cpp)

target_link_libraries(recognition algorithm_dummy ${GTKMM_LIBRARIES} ${Boost_LIBRARIES}) # ${OpenCV_LIBS}

