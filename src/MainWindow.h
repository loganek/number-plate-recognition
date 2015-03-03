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

	ModelColumns tree_columns;

	Glib::RefPtr<Gtk::ListStore> file_model;
	Glib::RefPtr<Gtk::ListStore> algorithm_model;

	std::shared_ptr<AlgorithmManager> manager;

	void on_selectDirButton_clicked();
	void on_runProcessingButton_clicked();

	void load_images_from_directory(const Glib::ustring& filename);
	std::shared_ptr<IAlgorithm> get_selected_algorithm() const;

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
	virtual ~MainWindow();

	void set_manager(std::shared_ptr<AlgorithmManager> manager);
};


#endif /* SRC_MAINWINDOW_H_ */
