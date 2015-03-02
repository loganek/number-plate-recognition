/*
 * main.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */
#include "MainWindow.h"

#include <gtkmm.h>


int main(int argc, char** argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "number.plate.recognition");
// todo:
	Glib::RefPtr<Gtk::Builder> builder =
			Gtk::Builder::create_from_file("/home/loganek/Documents/number-plate-recognition/src/ui/recognition.glade");

	MainWindow* wnd_handler;
	builder->get_widget_derived("mainWindow", wnd_handler);

	app->run(*wnd_handler);

	return 0;
}
