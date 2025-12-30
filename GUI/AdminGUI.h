#ifndef ADMINGUI_H
#define ADMINGUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <QAbstractItemView>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QShortcut>
#include <QKeySequence>

#include "../Service/Service.h"

class GUI;

class AdminGUI : public QWidget {
private:
	GUI& gui;
	Service& service;

	QTableWidget* table;
	QPushButton* addButton;
	QPushButton* deleteButton;
	QPushButton* updateButton;
	QPushButton* undoButton;
	QPushButton* redoButton;

public:
	explicit AdminGUI(Service& service, GUI& gui, QWidget* parent = nullptr);

	void populateTable();
	void handleDelete();
	void handleUpdate();
	void handleUndo();
	void handleRedo();
};



class AddMovieDialog : public QDialog {
	//Q_OBJECT

private:
	QLineEdit* titleEdit;
	QLineEdit* genreEdit;
	QLineEdit* yearEdit;
	QLineEdit* likesEdit;
	QLineEdit* trailerEdit;
	QPushButton* okButton;
	QPushButton* cancelButton;

public:
	explicit AddMovieDialog(QWidget* parent = nullptr);

	QString getTitle() const { return titleEdit->text(); }
	QString getGenre() const { return genreEdit->text(); }
	int getYear() const { return yearEdit->text().toInt(); }
	int getLikes() const { return likesEdit->text().toInt(); }
	QString getTrailer() const { return trailerEdit->text(); }
};



class UpdateMovieDialog : public QDialog {
private:
	QLineEdit* titleEdit;
	QLineEdit* genreEdit;
	QLineEdit* yearEdit;
	QLineEdit* likesEdit;
	QLineEdit* trailerEdit;
	QPushButton* okButton;
	QPushButton* cancelButton;

public:
	explicit UpdateMovieDialog(QWidget* parent = nullptr);

	QString getTitle() const { return titleEdit->text(); }
	QString getGenre() const { return genreEdit->text(); }
	int getYear() const { return yearEdit->text().toInt(); }
	int getLikes() const { return likesEdit->text().toInt(); }
	QString getTrailer() const { return trailerEdit->text(); }
};

#endif
