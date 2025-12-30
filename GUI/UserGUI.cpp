#include "UserGUI.h"
#include "AdminGUI.h"
#include "GUI.h"

UserGUI::UserGUI(Service &service, GUI &gui, QWidget *parent) : service(service), gui(gui), QWidget(parent) {
	auto userLayout = new QHBoxLayout(this);

	/* Watchlist table */
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

	this->populateTable();

	userLayout->addWidget(table);

	/* Button layout */

	auto buttonLayout = new QVBoxLayout;

	browseButton = new QPushButton("Browse for new movies");
	logButton = new QPushButton("Log the selected movie as watched");
	CSVButton = new QPushButton("View watchlist in CSV");
	HTMLButton = new QPushButton("View watchlist in HTML");

	buttonLayout->addWidget(browseButton, 0, Qt::AlignTop);
	buttonLayout->addWidget(logButton, 0, Qt::AlignTop);
	buttonLayout->addWidget(CSVButton, 0, Qt::AlignTop);
	buttonLayout->addWidget(HTMLButton, 0, Qt::AlignTop);

	connect(browseButton, &QPushButton::clicked, this, &UserGUI::handleBrowse);

	connect(logButton, &QPushButton::clicked, this, &UserGUI::handleLog);

	connect(CSVButton, &QPushButton::clicked, this, [this] () {
		this->service.getWatchlistCSV().open();
	});

	connect(HTMLButton, &QPushButton::clicked, this, [this] () {
		this->service.getWatchlistHTML().open();
	});

	viewButton = new QPushButton("View watchlist (using models)");
	buttonLayout->addWidget(viewButton, 0, Qt::AlignTop);
	connect(viewButton, &QPushButton::clicked, this, &UserGUI::handleView);

	buttonLayout->addStretch();
	userLayout->addLayout(buttonLayout);
}

void UserGUI::populateTable() {
	gui.loadUserFile("../user.txt");
	const auto& films = service.getWatchlist();

	table->clearContents();
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
	gui.saveUserFile("../user.txt");
}

void UserGUI::handleBrowse() {
	BrowseGenresDialog dialog(service, this);
	if (dialog.exec() == QDialog::Accepted) {
		gui.saveUserFile("../user.txt");
		populateTable();
	}
}

void UserGUI::handleLog() {
	int selectedRow = table->currentRow();
	if (selectedRow < 0) {
		QMessageBox::warning(this, "No selection!", "Please select a row to delete.");
		return;
	}
	try {
		auto films = this->service.getWatchlist();
		auto film = films[selectedRow];
		this->service.addToWatched(film);
		gui.saveUserFile("../user.txt");
		LogDialog dialog(film, service, this);
		if (dialog.exec() == QDialog::Accepted)
			service.likeFilm(film);
		gui.populateAll();
	} catch (const std::exception& e) {
		QMessageBox::warning(this, "Error!", e.what());
	}
}

BrowseGenresDialog::BrowseGenresDialog(Service& service, QWidget *parent) : service(service) {
	this->setWindowTitle("Browse");
	auto layout = new QVBoxLayout();

	auto label = new QLabel("Select the genre you wish to browse!");
	layout->addWidget(label);

	list = new QListWidget;
	auto genres = this->service.genres();
	for (const auto& genre : genres) {
		list->addItem(QString::fromStdString(genre));
	}

	list->setSelectionBehavior(QAbstractItemView::SelectRows);
	list->setSelectionMode(QAbstractItemView::SingleSelection);
	list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	list->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);
	layout->addWidget(list);

	auto button = new QPushButton("Browse selected genre");
	layout->addWidget(button);

	connect(button, &QPushButton::clicked, this, [this] () {
		int selectedRow = list->currentRow();
		if (selectedRow < 0) {
			QMessageBox::warning(this, "No selection!", "Please select a genre.");
			return;
		}
		try {
			auto item = list->item(selectedRow);
			auto genre = item->text().toStdString();
			const auto& films = this->service.getFilmsByGenre(genre);
			for (const auto& film : films) {
				BrowseFilmDialog dialog(film, this->service, this);
				if (dialog.exec() == QDialog::Rejected)
					break;
			}
			this->accept();
		} catch (const std::exception& e) {
			QMessageBox::warning(this, "Error!", e.what());
		}
	});

	this->setLayout(layout);
}

BrowseFilmDialog::BrowseFilmDialog(const Film& film, Service& service, QWidget *parent) : film(film), service(service) {
	this->setWindowTitle("How do you like this film?");
	auto layout = new QVBoxLayout;

	auto formLayout = new QFormLayout;
	auto titleEdit = new QLineEdit;
	auto genreEdit = new QLineEdit;
	auto yearEdit = new QLineEdit;
	auto likesEdit = new QLineEdit;
	titleEdit->setText(QString::fromStdString(film.getTitle()));
	genreEdit->setText(QString::fromStdString(film.getGenre()));
	yearEdit->setText(QString::number(film.getYear()));
	likesEdit->setText(QString::number(film.getLikes()));
	titleEdit->setReadOnly(true);
	genreEdit->setReadOnly(true);
	yearEdit->setReadOnly(true);
	likesEdit->setReadOnly(true);
	formLayout->addRow("Title:",titleEdit);
	formLayout->addRow("Genre:", genreEdit);
	formLayout->addRow("Year:", yearEdit);
	formLayout->addRow("Likes:", likesEdit);
	layout->addLayout(formLayout);

	auto trailerButton = new QPushButton("Open trailer");
	trailerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	layout->addWidget(trailerButton);

	auto buttonLayout = new QHBoxLayout;
	auto addButton = new QPushButton("Add to watchlist");
	auto skipButton = new QPushButton("Skip!");
	auto cancelButton = new QPushButton("Cancel");
	addButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	skipButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	cancelButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	buttonLayout->addWidget(addButton, 1);
	buttonLayout->addWidget(skipButton, 1);
	buttonLayout->addWidget(cancelButton, 1);
	layout->addLayout(buttonLayout);

	connect(trailerButton, &QPushButton::clicked, this, [film] () {
		GUI::openLink(film.getTrailer());
	});

	connect(addButton, &QPushButton::clicked, this, [this, film] () {
		this->service.addToWatchlist(film);
		this->accept();
	});

	connect(skipButton, &QPushButton::clicked, this, [this, film] () {
		this->service.addToRejected(film);
		this->accept();
	});

	connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

	setLayout(layout);
}


LogDialog::LogDialog(const Film &film, Service &service, QWidget *parent) : film(film), service(service) {
	this->setWindowTitle("Log film");
	auto layout = new QVBoxLayout;

	std::string labelString = "Glad you watched " + film.toString() + "!\nWould you like to give it a thumbs up?";
	auto label = new QLabel(QString::fromStdString(labelString));
	layout->addWidget(label);

	auto buttonLayout = new QHBoxLayout;
	auto yesButton = new QPushButton("Yes");
	auto noButton = new QPushButton("No");
	buttonLayout->addWidget(yesButton);
	buttonLayout->addWidget(noButton);
	layout->addLayout(buttonLayout);

	connect(yesButton, &QPushButton::clicked, this, &QDialog::accept);
	connect(noButton, &QPushButton::clicked, this, &QDialog::reject);

	setLayout(layout);
}

void UserGUI::handleView() {
	ViewDialog viewDialog(service);
	viewDialog.exec();
}

ViewDialog::ViewDialog(Service& service, QWidget *parent) : service(service) {
	this->setWindowTitle("Your watchlist");
	this->setMinimumSize(QSize(550, 400));
	auto layout = new QVBoxLayout();

	table = new QTableView();
	model = new WatchlistModel(service);
	table->setModel(model);

	layout->addWidget(table);
	this->setLayout(layout);
}