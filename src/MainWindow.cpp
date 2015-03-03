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

	builder->get_widget("runProcessingButton", runProcessingButton);
	runProcessingButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_runProcessingButton_clicked));

	builder->get_widget("processingOutputLabel", processingOutputLabel);
}

MainWindow::~MainWindow()
{
}

void MainWindow::set_manager(std::shared_ptr<AlgorithmManager> manager)
{
	this->manager = manager;
	algorithm_model->clear();

	for (auto algo : manager->get_algorithms())
	{
		auto row = *(algorithm_model->append());
		row[tree_columns.col_str_first] = algo;
	}
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

void MainWindow::on_runProcessingButton_clicked()
{
	auto algorithm = get_selected_algorithm();

	if (!algorithm)
		return;

	processingOutputLabel->set_text(algorithm->process(cv::Mat()));
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

std::shared_ptr<IAlgorithm> MainWindow::get_selected_algorithm() const
{
	Gtk::TreeModel::iterator iter = algorithmComboBox->get_active();
	std::shared_ptr<IAlgorithm> alg_empty;

	if (!iter)
		return alg_empty;

	Gtk::TreeModel::Row row = *iter;
	if (!row)
		return alg_empty;

	Glib::ustring name = row[tree_columns.col_str_first];

	return manager->get_algorithm(name);
}
