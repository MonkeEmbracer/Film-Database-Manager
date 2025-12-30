#include "AdminGUI.h"
#include "AdminGUI.h"
#include "GUI.h"

AdminGUI::AdminGUI(Service& service, GUI& gui, QWidget* parent) : service(service), gui(gui), QWidget(parent) {
	auto adminLayout = new QHBoxLayout(this);

	/* Film table */

	table = new QTableWidget{0, 5};
	QStringList headers{"Title", "Genre", "Year", "Likes", "Trailer"};
	table->setHorizontalHeaderLabels(headers);

	// For table resize behaviour
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

	// For scrolling in table
	table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	// For whole row selection
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::SingleSelection);

	gui.loadAdminFile("../admin.txt");
	this->populateTable();

	adminLayout->addWidget(table);

	/* Button layout */

	auto buttonLayout = new QVBoxLayout;

	addButton = new QPushButton("Add new movie");
	deleteButton = new QPushButton("Delete movie");
	updateButton = new QPushButton("Update movie");

	buttonLayout->addWidget(addButton, 0, Qt::AlignTop);
	buttonLayout->addWidget(deleteButton, 0, Qt::AlignTop);
	buttonLayout->addWidget(updateButton, 0, Qt::AlignTop);

	connect(addButton, &QPushButton::clicked, this, [this]() {
		AddMovieDialog dialog(this);
		if (dialog.exec() == QDialog::Accepted) {
			std::string title = dialog.getTitle().toStdString();
			std::string genre = dialog.getGenre().toStdString();
			auto year = dialog.getYear();
			auto likes = dialog.getLikes();
			std::string trailer = dialog.getTrailer().toStdString();

			try {
				this->service.add(Service::createFilm(title, genre, year, likes, trailer));
				this->gui.saveAdminFile("../admin.txt");
				this->gui.populateAll();
			} catch (const std::exception& e) {
				QMessageBox::warning(this, "Error!", e.what());
			}
		}
	});

	connect(deleteButton, &QPushButton::clicked, this, &AdminGUI::handleDelete);
	connect(updateButton, &QPushButton::clicked, this, &AdminGUI::handleUpdate);

	undoButton = new QPushButton("Undo");
	buttonLayout->addWidget(undoButton, 0, Qt::AlignTop);
	connect(undoButton, &QPushButton::clicked, this, &AdminGUI::handleUndo);
	auto undoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
	connect(undoShortcut, &QShortcut::activated, this, &AdminGUI::handleUndo);

	redoButton = new QPushButton("Redo");
	buttonLayout->addWidget(redoButton, 0, Qt::AlignTop);
	connect(redoButton, &QPushButton::clicked, this, &AdminGUI::handleRedo);
	auto redoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
	connect(redoShortcut, &QShortcut::activated, this, &AdminGUI::handleRedo);

	buttonLayout->addStretch();
	adminLayout->addLayout(buttonLayout);
}

void AdminGUI::populateTable() {
	table->clearContents();
	table->setRowCount(0);

	const auto& films = service.getAll();

	table->setRowCount(films.size());
	int row = 0;
	for (const auto& film : films) {
		table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(film.getTitle())));
		table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(film.getGenre())));
		table->setItem(row, 2, new QTableWidgetItem(QString::number(film.getYear())));
		table->setItem(row, 3, new QTableWidgetItem(QString::number(film.getLikes())));
		table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(film.getTrailer())));
		++row;
	}
}

void AdminGUI::handleDelete() {
	int selectedRow = table->currentRow();
	if (selectedRow < 0) {
		QMessageBox::warning(this, "No selection!", "Please select a row to delete.");
		return;
	}
	try {
		service.removeByIndex(selectedRow);
		gui.saveAdminFile("../admin.txt");
		gui.populateAll();
	} catch (const std::exception& e) {
		QMessageBox::warning(this, "Error!", e.what());
	}
}

void AdminGUI::handleUpdate() {
	int selectedRow = table->currentRow();
	if (selectedRow < 0) {
		QMessageBox::warning(this, "No selection!", "Please select a row to delete.");
		return;
	}
	try {
		UpdateMovieDialog dialog(this);
		if (dialog.exec() == QDialog::Accepted) {
			std::string title = dialog.getTitle().toStdString();
			std::string genre = dialog.getGenre().toStdString();
			int year = dialog.getYear();
			int likes = dialog.getLikes();
			std::string trailer = dialog.getTrailer().toStdString();

			try {
				service.update(selectedRow, Service::createFilm(title, genre, year, likes, trailer));
				gui.saveAdminFile("../admin.txt");
				gui.populateAll();
			} catch (const std::exception& e) {
				QMessageBox::warning(this, "Error!", e.what());
			}
		}
	} catch (const std::exception& e) {
		QMessageBox::warning(this, "Error!", e.what());
	}
}



AddMovieDialog::AddMovieDialog(QWidget *parent) : QDialog(parent) {
	this->setWindowTitle("Add movie");
	auto layout = new QFormLayout(this);

	titleEdit = new QLineEdit;
	genreEdit = new QLineEdit;
	yearEdit = new QLineEdit;
	likesEdit = new QLineEdit;
	trailerEdit = new QLineEdit;

	layout->addRow("Title:", titleEdit);
	layout->addRow("Genre:", genreEdit);
	layout->addRow("Year:", yearEdit);
	layout->addRow("Likes:", likesEdit);
	layout->addRow("Trailer:", trailerEdit);

	okButton = new QPushButton("OK");
	cancelButton = new QPushButton("Cancel");

	auto buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

	layout->addRow(buttonLayout);
}

UpdateMovieDialog::UpdateMovieDialog(QWidget *parent) {
	this->setWindowTitle("Update movie");
	auto layout = new QFormLayout(this);

	titleEdit = new QLineEdit;
	genreEdit = new QLineEdit;
	yearEdit = new QLineEdit;
	likesEdit = new QLineEdit;
	trailerEdit = new QLineEdit;

	layout->addRow("Title:", titleEdit);
	layout->addRow("Genre:", genreEdit);
	layout->addRow("Year:", yearEdit);
	layout->addRow("Likes:", likesEdit);
	layout->addRow("Trailer:", trailerEdit);

	okButton = new QPushButton("OK");
	cancelButton = new QPushButton("Cancel");

	auto buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

	layout->addRow(buttonLayout);
}

void AdminGUI::handleUndo() {
	try {
		service.undo();
		gui.saveAdminFile("../admin.txt");
		populateTable();
	} catch (const std::exception& e) {
		QMessageBox::warning(this, "Oopsie!", e.what());
	}
}

void AdminGUI::handleRedo() {
	try {
		service.redo();
		gui.saveAdminFile("../admin.txt");
		populateTable();
	} catch (const std::exception& e) {
		QMessageBox::warning(this, "Oopsie!", e.what());
	}
}