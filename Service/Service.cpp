#include "Service.h"
#include "../Exceptions/Exceptions.h"
#include <algorithm>
#include <fstream>

void Service::add(const Film& film) {
	/*
	 * Adds a new film into the database.
	 * Throws an exception if film is already in the database.
	 * param film: the film to be added
	 * return: none
	 */
	if (this->find(film))
		throw ServiceException("Film already in database!\n");
	this->repo.add(film);

	undoStack.push(std::make_unique<ActionAdd>(repo, film));
	while (!redoStack.empty()) redoStack.pop();
}

void Service::remove(const Film& film) {
	/*
	 * Removes a film from the database.
	 * Throws an exception if film is not in the database.
	 * param film: the film to be removed
	 * return: none
	 */
	if (!this->find(film))
		throw ServiceException("Film is not in database!\n");
	this->repo.remove(film);

	undoStack.push(std::make_unique<ActionRemove>(repo, film));
	while (!redoStack.empty()) redoStack.pop();
}

void Service::removeByIndex(int index) {
	/*
	 * Removes a film from the database by its index.
	 * Throws an exception if index is out of range.
	 * param index: the index of the film to be removed
	 * return: none
	 */
	if (index < 0 or index >= this->repo.getSize())
		throw ServiceException("Index is out of range!\n");
	auto film = repo.getAll()[index];
	this->repo.removeByIndex(index);

	undoStack.push(std::make_unique<ActionRemove>(repo, film));
	while (!redoStack.empty()) redoStack.pop();
}

void Service::update(int index, const Film& newFilm) {
	/*
	 * Updates a film at a given index with a new film object.
	 * Throws an exception if index is out of range.
	 * param index: the index of the film to be updated
	 * param newFilm: the new film to replace the old one
	 * return: none
	 */
	auto oldFilm = repo.getAll()[index];
	this->repo.update(index, newFilm);

	undoStack.push(std::make_unique<ActionUpdate>(repo, oldFilm, newFilm));
	while (!redoStack.empty()) redoStack.pop();
}

bool Service::find(const Film& film) const {
	/*
	 * Checks if a film exists in the database.
	 * param film: the film to search for
	 * return: true if film is found, false otherwise
	 */
	return this->repo.find(film);
}

const DynamicVector<Film>& Service::getAll() const {
	/*
	 * Returns a read-only iterable DynamicVector containing the data of the repository.
	 * return: as stated above
	 */
	return this->repo.getAll();
}

Film Service::createFilm(const std::string& title, const std::string& genre, const int year, const int likes, const std::string& trailer) {
	/*
	 * Creates a new film with the given details.
	 * param title: the title of the film
	 * param genre: the genre of the film
	 * param year: the release year of the film
	 * param likes: the number of likes the film has received
	 * param trailer: the URL of the film's trailer
	 * return: the created film object
	 */
	Film film;
	film.setTitle(title);
	film.setGenre(genre);
	film.setYear(year);
	film.setLikes(likes);
	film.setTrailer(trailer);
	return film;
}

DynamicVector<std::string> Service::genres() {
	/*
	 * Compiles all the unique genres in the database into one array.
	 * Ignores all genres for which all films are on the user's watchlist or have been already watched.
	 * If there are genres for which the only valid films have been rejected, the rejections are undone and the genres are appended.
	 * return: a DynamicVector containing the genres
	 */
	DynamicVector<std::string> genres;
	DynamicVector<std::string> rejectedGenres;
	auto films = this->getAll();
	const auto& watchlist = this->getWatchlist();
	const auto& rejected = this->getRejected();
	const auto& watched = this->getWatched();

	for (const auto& film : films) {
		if (std::find(watchlist.begin(), watchlist.end(), film) != watchlist.end() or
			std::find(rejected.begin(), rejected.end(), film) != rejected.end() or
			std::find(watched.begin(), watched.end(), film) != watched.end())
			continue;

		std::string genre = film.getGenre();
		if (std::find(genres.begin(), genres.end(), genre) == genres.end())
			genres.push_back(genre);
	}

	// check again for genres with rejected films that have not been added
	for (const auto& film : films) {
		if (std::find(watchlist.begin(), watchlist.end(), film) != watchlist.end() or
			std::find(watched.begin(), watched.end(), film) != watched.end())
			continue;

		std::string genre = film.getGenre();
		if (std::find(genres.begin(), genres.end(), genre) == genres.end()) {
			genres.push_back(genre);
			rejectedGenres.push_back(genre);
			this->watchlist->unreject(film);
		}
		else if (std::find(rejectedGenres.begin(), rejectedGenres.end(), genre) != rejectedGenres.end())
			this->watchlist->unreject(film);
	}
	return genres;
}

DynamicVector<Film> Service::getFilmsByGenre(const std::string& genre) const {
	/*
	 * Compiles all the valid films in the database having a given genre.
	 * param genre: the wanted genre
	 * return: a DynamicArray containing the wanted films
	 */
	DynamicVector<Film> filteredFilms;
	auto films = this->getAll();
	const auto& watchlist = this->getWatchlist();
	const auto& rejected = this->getRejected();
	const auto& watched = this->getWatched();
	for (const auto& film : films)
		if (film.getGenre() == genre and std::find(watchlist.begin(), watchlist.end(), film) == watchlist.end() and
			std::find(rejected.begin(), rejected.end(), film) == rejected.end() and
			std::find(watched.begin(), watched.end(), film) == watched.end())
			filteredFilms.push_back(film);
	return filteredFilms;
}

DynamicVector<Film> Service::getUnbrowsed() const {
	/*
	 * Compiles all the films from the database that have not been added to the watchlist, rejected, or watched.
	 * return: a DynamicArray containing the desired films
	 */
	DynamicVector<Film> filteredFilms;
	auto films = this->getAll();
	const auto& watchlist = this->getWatchlist();
	const auto& rejected = this->getRejected();
	const auto& watched = this->getWatched();
	for (const auto& film : films)
		if (std::find(watchlist.begin(), watchlist.end(), film) == watchlist.end() and
			std::find(rejected.begin(), rejected.end(), film) == rejected.end() and
			std::find(watched.begin(), watched.end(), film) == watched.end())
			filteredFilms.push_back(film);
	return filteredFilms;
}

void Service::addToWatchlist(const Film& film) {
	/*
	 * Adds a new film to the watchlist.
	 * param film: the film to be added
	 * return: none
	 */
	this->watchlist->add(film);
}

void Service::addToRejected(const Film& film) {
	/*
	 * Adds a new film to the rejection list, in case the user has decided not to add it to their watchlist.
	 * param film: the film to be added
	 * return: none
	 */
	this->watchlist->reject(film);
}

void Service::addToWatched(const Film& film) {
	/*
	 * Adds a new film to the watched list.
	 * param film: the film to be added
	 * return: none
	 */
	this->watchlist->watch(film);
}

const DynamicVector<Film>& Service::getWatchlist() const {
	return this->watchlist->getWatchlist();
}

const DynamicVector<Film>& Service::getRejected() const {
	return this->watchlist->getRejected();
}

const DynamicVector<Film>& Service::getWatched() const {
	return this->watchlist->getWatched();
}

void Service::likeFilm(Film& film) {
	/*
	 * Increments the like count of a film in the database.
	 * Called when the user decides to like a film.
	 * param film: the liked film
	 * return: none
	 */
	auto films = this->getAll();
	int index = 0;
	for (const auto& oldFilm : films) {
		if (oldFilm == film) {
			film.setLikes(film.getLikes() + 1);
			this->update(index, film);
		}
		++index;
	}
}

void Service::clearWatchlist() {
	/*
	 * Completely erases the user data stored in the RAM.
	 * return: none
	 */
	this->watchlist->clear();
}

void Service::switchMode() {
	mode = !mode;
	if (mode) {
		watchlist = &watchlistHTML;
		otherFile = "../watchlist.html";
	}
	else {
		watchlist = &watchlistCSV;
		otherFile = "../watchlist.csv";
	}
}

void Service::saveUserFile(const std::string& filename) {
	this->watchlist->saveFile(filename);
	this->switchMode();
	this->loadUserFile(filename);
	this->watchlist->saveFile(filename);
}

void Service::loadUserFile(const std::string& filename) {
	this->clearWatchlist();

	std::ifstream file(filename);
	auto data = this->getAll();
	int count = 0;

	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		if (std::find(data.begin(), data.end(), film) != data.end())
			this->addToWatchlist(film);
	}

	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		if (std::find(data.begin(), data.end(), film) != data.end())
			this->addToRejected(film);
	}

	file >> count;
	file.get();
	for (int i = 0; i < count; ++i) {
		Film film;
		file >> film;
		if (std::find(data.begin(), data.end(), film) != data.end())
			this->addToWatched(film);
	}
}

void Service::seeWatchlist() const {
	this->watchlist->open();
}

void Service::undo() {
	if (!undoStack.empty()) {
		auto action = std::move(undoStack.top());
		undoStack.pop();
		action->executeUndo();
		redoStack.push(std::move(action));
	}
	else
		throw std::logic_error("Nothing to undo");
}

void Service::redo() {
	if (!redoStack.empty()) {
		auto action = std::move(redoStack.top());
		redoStack.pop();
		action->executeRedo();
		undoStack.push(std::move(action));
	}
	else
		throw std::logic_error("Nothing to redo");
}

void Service::clearUndoRedo() {
	while (!undoStack.empty()) undoStack.pop();
	while (!redoStack.empty()) redoStack.pop();
}
