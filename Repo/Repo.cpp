#include "Repo.h"
#include "../Exceptions/Exceptions.h"
#include <fstream>
#include <algorithm>

void Repo::add(const Film& film) {
	/*
	 * Adds a new film into the repository.
	 * param film: the film to be added
	 * return: none
	 */
	this->data.push_back(film);
}

void Repo::remove(const Film& film) {
	/*
	 * Removes the specified film from the repository.
	 * param film: the film to be removed from the repository
	 * return: none
	 */
	this->data.erase(std::remove(this->data.begin(), this->data.end(), film), this->data.end());
}

void Repo::removeByIndex(const int index) {
	/*
	 * Removes a film from the repository by its index.
	 * param index: the index of the film to be removed
	 * return: none
	 */
	this->data.erase(this->data.begin() + index);
}

void Repo::update(const int index, const Film& newFilm) {
	/*
	 * Updates a film at a given index with a new film object.
	 * Throws an exception if the index is out of range.
	 * param index: the index of the film to be updated
	 * param newFilm: the new film object that will replace the old one
	 * return: none
	 */
	if (index < 0 or index >= this->data.size())
		throw RepoException("Index out of range.\n");
	const Film oldFilm = this->data[index];

	this->remove(oldFilm);
	this->add(newFilm);
}

bool Repo::find(const Film& film) const {
	/*
	 * Checks if a film exists in the repository.
	 * param film: the film to search for in the repository
	 * return: true if the film is found, false otherwise
	 */
	return std::find(this->data.begin(), this->data.end(), film) != this->data.end();
}


const DynamicVector<Film>& Repo::getAll() const {
	/*
	 * Returns a read-only iterable DynamicVector containing the data of the repository.
	 * return: as stated above
	 */
	return this->data;
}

int Repo::getSize() const {
	return this->data.size();
}
