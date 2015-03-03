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

	ModelColumns tree_columns;

	Glib::RefPtr<Gtk::ListStore> file_model;
	Glib::RefPtr<Gtk::ListStore> algorithm_model;

	std::shared_ptr<AlgorithmManager> manager;
	std::string current_directory;

	void on_selectDirButton_clicked();
	void on_filesTreeView_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void on_algorithmComboBox_changed();

	void run_processing();
	std::string get_selected_filename() const;
	void load_images_from_directory();
	std::shared_ptr<IAlgorithm> get_selected_algorithm() const;
	void load_input_file(const std::string& filename);

	void show_message(const std::string& header, const std::string& msg, Gtk::MessageType type) const;

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
	virtual ~MainWindow();

	void set_manager(std::shared_ptr<AlgorithmManager> manager);
	void set_current_directory(const std::string& dirpath);
};


#endif /* SRC_MAINWINDOW_H_ */
