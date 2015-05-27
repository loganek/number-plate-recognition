add_library(algorithms 
	dummy_algorithm/DummyAlgorithm.cpp
	rzut_jasnosci/RzutJasnosci.cpp
	szczyty_histogramu/SzczytyHistogramu.cpp
	utils.cpp)

target_link_libraries(algorithms ${OpenCV_LIBS})

