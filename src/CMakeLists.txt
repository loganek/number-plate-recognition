add_executable(recognition main.cpp MainWindow.cpp AlgorithmManager.cpp)

target_link_libraries(recognition ${GTKMM_LIBRARIES} ${Boost_LIBRARIES} ) # ${OpenCV_LIBS}

