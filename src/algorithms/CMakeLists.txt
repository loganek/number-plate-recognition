add_library(algorithms 
	dummy_algorithm/DummyAlgorithm.cpp
#	rzut_jasnosci/RzutJasnosci.cpp
#	szczyty_histogramu/SzczytyHistogramu.cpp
#	counting_lines/CLRzutJasnosci.cpp
	EndpointAlgorithm.cpp
	endpoints/SzczytyHistogramu.cpp
	endpoints/SauvolPietikainen.cpp
	endpoints/RzutJasnosciRegionGrowing.cpp
	endpoints/Canny.cpp
	utils.cpp)

target_link_libraries(algorithms ${OpenCV_LIBS})
target_link_libraries(algorithms -ltesseract)

