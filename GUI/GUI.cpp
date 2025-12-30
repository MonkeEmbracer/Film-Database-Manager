#include "GUI.h"
#include <fstream>
#include <cstdlib>
#include <algorithm>

GUI::GUI(QWidget* parent) : QWidget(parent) {
	this->resize(900, 600);
	this->setMinimumSize(900, 600);
	this->setWindowTitle("Bogdan's Film Management App");

	adminGUI = new AdminGUI(this->service, *this);
	userGUI = new UserGUI(this->service, *this);

	tabWidget = new QTabWidget(this);
	tabWidget->addTab(adminGUI, "Admin");
	tabWidget->addTab(userGUI, "User");

	service.clearUndoRedo();
	this->show();
}

void GUI::openLink(const std::string& url) {
	std::string command = "/mnt/c/Program\\ Files/BraveSoftware/Brave-Browser/Application/brave.exe " + url;
	system(command.c_str());
}

void GUI::saveAdminFile(const std::string& filename) const {
	std::ofstream file(filename);
	auto data = this->service.getAll();
	file << data.size() << '\n';
	for (const auto& film : data) {
		file << film.getTitle() << '\n';
		file << film.getGenre() << '\n';
		file << film.getYear() << '\n';
		file << film.getLikes() << '\n';
		file << film.getTrailer() << '\n';
	}
	file.close();
}

void GUI::loadAdminFile(const std::string& filename) {
	std::ifstream file(filename);
	int count = 0;
	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		this->service.add(film);
	}
	file.close();
}

void GUI::loadUserFile(const std::string& filename) {
	this->service.loadUserFile(filename);
}

void GUI::saveUserFile(const std::string& filename) {
	this->service.saveUserFile(filename);
}

void GUI::populateAll() {
	adminGUI->populateTable();
	userGUI->populateTable();
}