/*
 * MainWindow.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#include "MainWindow.h"

#include <boost/filesystem.hpp>

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::Window(cobject)
{
	builder->get_widget("selectDirButton", selectDirButton);
	selectDirButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_selectDirButton_clicked));

	builder->get_widget("dirEntry", dirEntry);

	builder->get_widget("filesTreeView", filesTreeView);
	file_model = Gtk::ListStore::create(tree_columns);
	filesTreeView->set_model(file_model);
	filesTreeView->append_column("File Name", tree_columns.col_str_first);

	builder->get_widget("algorithmComboBox", algorithmComboBox);
	algorithm_model = Gtk::ListStore::create(tree_columns);
	algorithmComboBox->set_model(algorithm_model);
	algorithmComboBox->pack_start(tree_columns.col_str_first);

	for (auto algo : manager.get_algorithms())
	{
		auto row = *(algorithm_model->append());
		row[tree_columns.col_str_first] = algo;
	}
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_selectDirButton_clicked()
{
	Gtk::FileChooserDialog dialog("Please choose a directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	if (dialog.run() == Gtk::RESPONSE_OK)
	{
		auto filename = dialog.get_filename();
		dirEntry->set_text(filename);
		load_images_from_directory(filename);
	}
}

void MainWindow::load_images_from_directory(const Glib::ustring& filename)
{
	namespace fs = boost::filesystem;
	fs::path dir(filename);
	fs::directory_iterator end_iter;

	if (!fs::exists(dir) || !fs::is_directory(dir))
		return;

	for(fs::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter)
	{
		if (fs::is_regular_file(dir_iter->status()) &&
				std::find(image_extensions.begin(), image_extensions.end(), fs::extension(dir_iter->path())) != image_extensions.end())
		{
			Gtk::TreeModel::Row row = *(file_model->append());
			row[tree_columns.col_str_first] = dir_iter->path().string();
		}
	}
}
