#ifndef FILM_H
#define FILM_H

#include <iostream>
#include <string>

class Film {
private:
	std::string title;
	std::string genre;
	int year = 0;
	int likes = 0;
	std::string trailer;

public:
	const std::string& getTitle() const {return this->title;}
	const std::string& getGenre() const {return this->genre;}
	int getYear() const {return this->year;}
	int getLikes() const {return this->likes;}
	const std::string& getTrailer() const {return this->trailer;}

	void setTitle(const std::string& newTitle) {this->title = newTitle;}
	void setGenre(const std::string& newGenre) {this->genre = newGenre;}
	void setYear(const int newYear) {this->year = newYear;}
	void setLikes(const int newLikes) {this->likes = newLikes;}
	void setTrailer(const std::string& newTrailer) {this->trailer = newTrailer;}

	Film() = default;
	inline Film(const std::string&, const std::string&, int, int, const std::string&);
	inline Film& operator=(const Film&);
	inline bool operator==(const Film&) const;
	inline bool operator!=(const Film&) const;
	friend inline std::ostream& operator<<(std::ostream&, const Film&);
	friend inline std::istream& operator>>(std::istream&, Film&);
	inline std::string toString() const;
};

Film::Film(const std::string& title, const std::string& genre, const int year, const int likes, const std::string& trailer) {
	this->title = title;
	this->genre = genre;
	this->year = year;
	this->likes = likes;
	this->trailer = trailer;
}

Film& Film::operator=(const Film& film) {
	this->title = film.getTitle();
	this->genre = film.getGenre();
	this->year = film.getYear();
	this->likes = film.getLikes();
	this->trailer = film.getTrailer();
	return *this;
}

bool Film::operator==(const Film& film) const {
	return this->title == film.getTitle() and this->year == film.getYear();
}

bool Film::operator!=(const Film& film) const {
	return this->title != film.getTitle() or this->year != film.getYear();
}

std::istream& operator>>(std::istream& is, Film& film) {
	std::string title, genre, trailer;
	int year, likes;
	std::getline(is, title);
	std::getline(is, genre);
	is >> year;
	is >> likes;
	is.get();
	std::getline(is, trailer);
	film = Film(title, genre, year, likes, trailer);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Film& film) {
	os << film.getTitle() << '\n';
	os << film.getGenre() << '\n';
	os << film.getYear() << '\n';
	os << film.getLikes() << '\n';
	os << film.getTrailer() << '\n';
	return os;
}

inline std::string Film::toString() const {
	return this->title + " (" + std::to_string(this->year) + ")";
}


#endif
