/*
 * main.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */
#include "MainWindow.h"
#include "algorithms/dummy_algorithm/DummyAlgorithm.h"
#include "algorithms/rzut_jasnosci/RzutJasnosci.h"
#include "algorithms/szczyty_histogramu/SzczytyHistogramu.h"

#include <gtkmm.h>


int main(int argc, char** argv)
{
	// gtkmm bug workaround
	int argc_t = 1;
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc_t, argv, "number.plate.recognition");
// todo relative path instead of absolute:
	Glib::RefPtr<Gtk::Builder> builder =
			Gtk::Builder::create_from_file("/home/loganek/Dokumenty/number-plate-recognition/src/tuner/ui/tuner.glade");

	std::initializer_list<std::shared_ptr<IAlgorithm>> list = {
			std::make_shared<DummyAlgorithm>(),
			std::make_shared<RzutJasnosci>(),
			std::make_shared<SzczytyHistogramu>()
	};
	auto manager = std::make_shared<AlgorithmManager>(list);

	MainWindow* wnd_handler;
	builder->get_widget_derived("mainWindow", wnd_handler);
	wnd_handler->set_manager(manager);

	if (argc > 1)
		wnd_handler->set_current_directory(argv[1]);

	app->run(*wnd_handler);

	return 0;
}
