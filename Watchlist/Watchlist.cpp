#include "Watchlist.h"
#include <algorithm>
#include <fstream>
#include <cstdlib>

void Watchlist::add(const Film& film) {
	/*
	 * Adds a new film to the watchlist.
	 * param film: the film to be added
	 * return: none
	 */
	this->data.push_back(film);
}

void Watchlist::reject(const Film& film) {
	/*
	 * Adds a new film to the rejection list, in case the user has decided not to add it to their watchlist.
	 * param film: the film to be added
	 * return: none
	 */
	this->rejected.push_back(film);
}

void Watchlist::unreject(const Film& film) {
	/*
	 * Removes a film from the rejection list.
	 * param film: the film to be removed
	 * return: none
	 */
	this->rejected.erase(std::remove(this->rejected.begin(), this->rejected.end(), film), this->rejected.end());
}

void Watchlist::watch(const Film& film) {
	/*
	 * Adds a new film to the watched list.
	 * param film: the film to be added
	 * return: none
	 */
	this->data.erase(std::remove(this->data.begin(), this->data.end(), film), this->data.end());
	this->watched.push_back(film);
}

void Watchlist::clear() {
	/*
	 * Completely erases the user data stored in the RAM.
	 * return: none
	 */
	this->data.clear();
	this->rejected.clear();
	this->watched.clear();
}

const DynamicVector<Film>& Watchlist::getWatchlist() const {
	return this->data;
}

const DynamicVector<Film>& Watchlist::getRejected() const {
	return this->rejected;
}

const DynamicVector<Film>& Watchlist::getWatched() const {
	return this->watched;
}

void Watchlist::saveFile(const std::string& filename) {
	std::ofstream file(filename);
	auto watchlist = this->getWatchlist();
	auto rejected = this->getRejected();
	auto watched = this->getWatched();

	file << watchlist.size() << '\n';
	for (const auto& film : watchlist)
		file << film;

	file << rejected.size() << '\n';
	for (const auto& film : rejected)
		file << film;

	file << watched.size() << '\n';
	for (const auto& film : watched)
		file << film;

	file.close();
}



void WatchlistCSV::saveFile(const std::string& filename) {
	Watchlist::saveFile(filename);

	std::ofstream file(otherFile);
	for (const auto& film : this->data)
		file << film.getTitle() << ',' << film.getGenre() << ',' << film.getYear() << ',' << film.getLikes() << ','
		<< film.getTrailer() << '\n';
	file.close();
}

void WatchlistCSV::open() {
	std::string command = "cmd.exe /C start \"\" \"excel.exe\" \"D:\\School\\Object-Oriented Programming\\a10-MonkeEmbracer-1\\watchlist.csv\"";
	std::system(command.c_str());
}


void WatchlistHTML::saveFile(const std::string& filename) {
	Watchlist::saveFile(filename);

	std::string line;
	std::ifstream fin;
	std::ofstream fout;
	fout.open(otherFile);

	fin.open("../template1.html");
	while (std::getline(fin, line))
		fout << line << '\n';
	fin.close();

	for (const auto& film : this->data) {
		fout << "\t<tr>\n";
		fout << "\t\t<td>" << film.getTitle() << "</td>\n";
		fout << "\t\t<td>" << film.getGenre() << "</td>\n";
		fout << "\t\t<td>" << film.getYear() << "</td>\n";
		fout << "\t\t<td>" << film.getLikes() << "</td>\n";
		fout << "\t\t<td><a href=\"" << film.getTrailer() << "\">Link</a></td>\n";
		fout << "\t</tr>\n";
	}

	fin.open("../template2.html");
	while (std::getline(fin, line))
		fout << line << '\n';
	fin.close();

	fout.close();
}

void WatchlistHTML::open() {
	const char* htmlPath = R"(D:\School\Object-Oriented Programming\a10-MonkeEmbracer-1\watchlist.html)";
	std::string command = "cmd.exe /c start \"\" \"" + std::string(htmlPath) + "\"";
	std::system(command.c_str());
}
