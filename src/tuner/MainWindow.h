/*
 * MainWindow.h
 *
 *  Created on: Mar 2, 2015
 *      Author: loganek
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include "AlgorithmManager.h"

#include <gtkmm.h>
#include <memory>
#include <map>

class ModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:

	ModelColumns()
	{ add(col_str_first);}

	Gtk::TreeModelColumn<Glib::ustring> col_str_first;
};

class MainWindow : public Gtk::Window
{
	std::vector<std::string> image_extensions = {".jpg", ".png", ".bmp", ".gif"};

	Gtk::Button *selectDirButton;
	Gtk::TreeView *filesTreeView;
	Gtk::Entry *dirEntry;
	Gtk::ComboBox *algorithmComboBox;
	Gtk::Button *runProcessingButton;
	Gtk::Label *processingOutputLabel;
	Gtk::Image *inputImage;
	Gtk::Image *outputImage;
	Gtk::Statusbar *mainStatusbar;
	Gtk::Viewport *algorithmViewport;
	Gtk::Entry *outputPathEntry;
	Gtk::CheckButton *browseModeCheckButton;

	Gtk::Button *startForAllButton;

	ModelColumns tree_columns;

	Glib::RefPtr<Gtk::ListStore> file_model;
	Glib::RefPtr<Gtk::ListStore> algorithm_model;

	std::shared_ptr<AlgorithmManager> manager;
	std::string current_directory;

	std::map<std::string, std::string> process_outputs; /* filename, output */

	void on_selectDirButton_clicked();
	void on_filesTreeView_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void on_algorithmComboBox_changed();

	void run_processing();
	std::string run_algorithm(const std::shared_ptr<IAlgorithm>& algorithm, const std::string& filename);
	std::string get_full_file_path(const std::string& dirname, const std::string &filename) const;

	std::string get_selected_file_path() const;
	void load_images_from_directory();
	std::shared_ptr<IAlgorithm> get_selected_algorithm() const;
	void load_input_file(const std::string& filename);

	void show_message(const std::string& header, const std::string& msg, Gtk::MessageType type) const;

	void process_all();

	void load_full_info(const std::string& filename);

	std::string get_selected_filename() const;

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
	virtual ~MainWindow();

	void set_manager(std::shared_ptr<AlgorithmManager> manager);
	void set_current_directory(const std::string& dirpath);
};


#endif /* SRC_MAINWINDOW_H_ */
