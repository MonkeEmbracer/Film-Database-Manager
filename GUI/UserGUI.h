#ifndef USERGUI_H
#define USERGUI_H

#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QListWidget>

#include "WatchlistModel.h"
#include "../Service/Service.h"

class GUI;

class UserGUI : public QWidget {
private:
	GUI& gui;
	Service& service;

	QTableWidget* table;
	QPushButton* browseButton;
	QPushButton* logButton;
	QPushButton* CSVButton;
	QPushButton* HTMLButton;
	QPushButton* viewButton;



public:
	explicit UserGUI(Service& service, GUI& gui, QWidget* parent = nullptr);

	void populateTable();
	void handleBrowse();
	void handleLog();
	void handleView();
};


class BrowseGenresDialog : public QDialog {
private:
	Service& service;
	QListWidget* list;

public:
	explicit BrowseGenresDialog(Service& service, QWidget* parent = nullptr);
};


class BrowseFilmDialog : public QDialog {
private:
	Film film;
	Service& service;

public:
	explicit BrowseFilmDialog(const Film& film, Service& service, QWidget* parent = nullptr);
};


class LogDialog : public QDialog {
private:
	Film film;
	Service& service;

public:
	explicit LogDialog(const Film& film, Service& service, QWidget* parent = nullptr);
};


class ViewDialog : public QDialog {
private:
	Service& service;

	QTableView* table;
	WatchlistModel* model;

public:
	explicit ViewDialog(Service& service, QWidget* parent = nullptr);
};

#endif
