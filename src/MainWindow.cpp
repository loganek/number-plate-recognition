/*
 * MainWindow.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#include "MainWindow.h"

#include "TimeMeasure.h"

#include <iomanip>

#include <boost/filesystem.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fs = boost::filesystem;

template <typename T>
static std::string to_string_with_precision(const T val, const int n = 6)
{
	std::ostringstream ss;
	ss << std::setprecision(n) << val;
	return ss.str();
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::Window(cobject)
{
	builder->get_widget("selectDirButton", selectDirButton);
	selectDirButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_selectDirButton_clicked));

	builder->get_widget("dirEntry", dirEntry);
	dirEntry->signal_activate().connect([this] { current_directory = dirEntry->get_text(); load_images_from_directory(); });

	builder->get_widget("filesTreeView", filesTreeView);
	file_model = Gtk::ListStore::create(tree_columns);
	filesTreeView->set_model(file_model);
	filesTreeView->append_column("File Name", tree_columns.col_str_first);
	filesTreeView->signal_row_activated().connect(sigc::mem_fun(*this, &MainWindow::on_filesTreeView_row_activated));

	builder->get_widget("algorithmComboBox", algorithmComboBox);
	algorithm_model = Gtk::ListStore::create(tree_columns);
	algorithmComboBox->set_model(algorithm_model);
	algorithmComboBox->pack_start(tree_columns.col_str_first);

	builder->get_widget("runProcessingButton", runProcessingButton);
	runProcessingButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_runProcessingButton_clicked));

	builder->get_widget("inputImage", inputImage);

	builder->get_widget("outputImage", outputImage);

	builder->get_widget("mainStatusbar", mainStatusbar);

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
		auto iter = algorithm_model->append();
		(*iter)[tree_columns.col_str_first] = algo;

		algorithmComboBox->set_active(iter);
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
		current_directory = filename;
		load_images_from_directory();
	}
}

void MainWindow::on_runProcessingButton_clicked()
{
	auto algorithm = get_selected_algorithm();

	if (!algorithm)
	{
		show_message("No algorithm selected",
				"Before running processing, an algorithm has to be chosen",
				Gtk::MESSAGE_ERROR);
		return;
	}

	std::string filename = get_selected_filename();

	if (filename.empty())
	{
		show_message("No image selected",
				"Before running processing, an image has to be chosen",
				Gtk::MESSAGE_ERROR);
		return;
	}
	std::string output;
	auto time = MEASURE_TIME(std::chrono::microseconds,
			output = algorithm->process(cv::imread(filename));
	);

	mainStatusbar->push("Execution time: " + to_string_with_precision(time/1000.0, 3) + " microseconds");

	processingOutputLabel->set_text(output);
	auto out_img = algorithm->get_output_image();
	outputImage->set(Gdk::Pixbuf::create_from_data(out_img.data, Gdk::COLORSPACE_RGB, false, 8, out_img.cols, out_img.rows, out_img.step));
	outputImage->queue_draw();
}

void MainWindow::load_images_from_directory()
{
	fs::path dir(current_directory);
	fs::directory_iterator end_iter;

	if (!fs::exists(dir) || !fs::is_directory(dir))
	{
		show_message("Directory does not exists",
				"Directory " + dir.string() + " does not exists",
				Gtk::MESSAGE_ERROR);
		return;
	}

	for(fs::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter)
	{
		if (fs::is_regular_file(dir_iter->status()) &&
				std::find(image_extensions.begin(), image_extensions.end(), fs::extension(dir_iter->path())) != image_extensions.end())
		{
			Gtk::TreeModel::Row row = *(file_model->append());
			row[tree_columns.col_str_first] = dir_iter->path().filename().string();
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

std::string MainWindow::get_selected_filename() const
{
	std::string ret;
	auto selection = filesTreeView->get_selection();
	if (!selection)
		return ret;

	auto iter = selection->get_selected();
	if (!iter)
		return ret;

	Glib::ustring f = iter->operator [](tree_columns.col_str_first);
	return (fs::path(current_directory) / fs::path(f)).string();
}

void MainWindow::on_filesTreeView_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
	load_input_file(get_selected_filename());
}

void MainWindow::load_input_file(const std::string& filename)
{
	inputImage->set(filename);
}

void MainWindow::show_message(const std::string& header, const std::string& msg, Gtk::MessageType type) const
{
	Gtk::MessageDialog dialog(header, false, type, Gtk::BUTTONS_OK);
	dialog.set_secondary_text(msg);
	dialog.run();
}
