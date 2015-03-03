include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tuner main.cpp MainWindow.cpp AlgorithmManager.cpp)

target_link_libraries(tuner algorithm_dummy ${GTKMM_LIBRARIES} ${Boost_LIBRARIES}) # ${OpenCV_LIBS}

